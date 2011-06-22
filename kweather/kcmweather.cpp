/*
  This file is part of KWeather.
  Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  As a special exception, permission is given to link this program
  with any edition of TQt, and distribute the resulting executable,
  without including the source code for TQt in the source distribution.
*/

#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqlabel.h>
#include <tqlayout.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kurlrequester.h>
#include <kcombobox.h>
#include <kcolorbutton.h>

#include "dockwidget.h"
#include "prefdialogdata.h"

#include "kcmweather.h"
#include "weatherservice_stub.h"

extern "C"
{
  KDE_EXPORT KCModule *create_weather( TQWidget *tqparent, const char * )
  {
    return new KCMWeather( tqparent, "kweather" );
  }
}

KCMWeather::KCMWeather( TQWidget *tqparent, const char *name )
  : KCModule( tqparent, name )
{
  mWeatherService = new WeatherService_stub( "KWeatherService",
      "WeatherService" );
  TQVBoxLayout *tqlayout = new TQVBoxLayout( this );
  mWidget = new prefsDialogData( this );

  mWidget->m_reportLocation->setFocus();
  tqlayout->addWidget( mWidget );
  tqlayout->addStretch();

  fillStationList();
  load();
  connect( mWidget->m_enableLog, TQT_SIGNAL( toggled( bool ) ),
                                 TQT_SLOT( enableLogWidgets( bool ) ) );
  connect( mWidget->m_viewMode, TQT_SIGNAL( released( int ) ),
                                TQT_SLOT( changeViewMode( int ) ) );
  connect( mWidget->m_reportLocation, TQT_SIGNAL( activated( const TQString& ) ),
                                      TQT_SLOT( reportLocationChanged() ) );
  connect( mWidget->m_textColor, TQT_SIGNAL( changed(const TQColor &) ),
                                 TQT_SLOT( textColorChanged(const TQColor &) ) );

  KAboutData *about = new KAboutData(
      I18N_NOOP( "kcmweather" ),
      I18N_NOOP( "KWeather Configure Dialog" ),
      0, 0, KAboutData::License_GPL,
      I18N_NOOP( "(c), 2003 Tobias Koenig" ) );

  about->addAuthor( "Tobias Koenig", 0, "tokoe@kde.org" );
  setAboutData(about);
}

KCMWeather::~KCMWeather()
{
  delete mWeatherService;
}
void KCMWeather::showEvent( TQShowEvent * )
{
  fillStationList();
}

void KCMWeather::fillStationList()
{
  // store current selection
  TQString current = mWidget->m_reportLocation->currentText();

  mWidget->m_reportLocation->clear();

  TQStringList stationList = mWeatherService->listStations();
  TQStringList::Iterator idx = stationList.begin();

  // get station name from station id for sorting afterwards
  for(; idx != stationList.end(); ++idx)
    *idx = mWeatherService->stationName(*idx);

  stationList.sort();

  idx = stationList.begin();
  for(; idx != stationList.end(); ++idx)
    mWidget->m_reportLocation->insertItem(*idx);

  // restore previous selection
  if ( current.isEmpty() )
  {
    // nothing defined yet; show this situation to the user, otherwise
    // he will see the first available setting which is not what he selected to view
    mWidget->m_reportLocation->insertItem("");
    mWidget->m_reportLocation->setCurrentText("");
  }
  else
  {
    for (int i = 0; i < mWidget->m_reportLocation->count(); i++)
      if (  mWidget->m_reportLocation->text(i) == current )
      {
        mWidget->m_reportLocation->setCurrentItem(i);
        break;
      }
  }

  if ( current != mWidget->m_reportLocation->currentText() )
    reportLocationChanged();
}

void KCMWeather::changeViewMode( int mode )
{
  mViewMode = mode;
  emit changed( true );
}

void KCMWeather::enableLogWidgets( bool value )
{
  mWidget->m_logFile->setEnabled( value );
  mWidget->m_labelLogFile->setEnabled( value );

  emit changed( true );
}

void KCMWeather::reportLocationChanged()
{
  kdDebug() << "New station: " << mWidget->m_reportLocation->currentText()
      << " Code: " << mWeatherService->stationCode( mWidget->m_reportLocation->currentText() ) << endl;
  emit changed( true );
}

void KCMWeather::textColorChanged(const TQColor &)
{
  emit changed( true );
}

void KCMWeather::load()
{
  kdDebug() << "Load" << endl;
  KConfig config( "weather_panelappletrc" );

  config.setGroup( "General Options" );
  bool enabled = config.readBoolEntry( "logging", false );
  mWidget->m_enableLog->setChecked( enabled );
  enableLogWidgets( enabled );

  static TQColor black(TQt::black);
  TQColor textColor = config.readColorEntry("textColor", &black);
  mWidget->m_textColor->setColor(textColor);

  TQString loc =  config.readEntry( "report_location" );

  mWidget->m_logFile->setURL( config.readPathEntry( "log_file_name" ) );

  if ( ! loc.isEmpty() )
    mWidget->m_reportLocation->setCurrentText( mWeatherService->stationName( loc ) );

  mWidget->m_viewMode->setButton( config.readNumEntry( "smallview_mode", dockwidget::ShowAll ) );
  changeViewMode( config.readNumEntry( "smallview_mode", dockwidget::ShowAll ) );
  emit changed( false );
}

void KCMWeather::save()
{
  kdDebug() << "Save" << endl;
  KConfig config( "weather_panelappletrc" );

  config.setGroup( "General Options" );
  config.writeEntry( "logging", mWidget->m_enableLog->isChecked() );
  config.writeEntry( "log_file_name", mWidget->m_logFile->url() );
  config.writeEntry( "textColor", mWidget->m_textColor->color() );

  // Station idx to local idx; if nothing selected yet, keep it empty
  TQString loc;
  if ( ! mWidget->m_reportLocation->currentText().isEmpty() )
    loc = mWeatherService->stationCode( mWidget->m_reportLocation->currentText() );
  config.writeEntry( "report_location", loc);

  config.writeEntry( "smallview_mode", mViewMode );
  config.sync();

  emit changed( false );
}

void KCMWeather::defaults()
{
  mWidget->m_enableLog->setChecked( false );
  enableLogWidgets( false );

  mWidget->m_logFile->setURL( "" );
  mWidget->m_reportLocation->setCurrentText( "" );
  changeViewMode( dockwidget::ShowAll );

  emit changed( true );
}

#include "kcmweather.moc"
