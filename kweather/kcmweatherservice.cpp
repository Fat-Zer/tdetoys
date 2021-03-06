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

#include <tqlayout.h>

#include <tdeaboutdata.h>
#include <kdebug.h>
#include <tdelocale.h>

#include "serviceconfigwidget.h"

#include "kcmweatherservice.h"

extern "C"
{
  KDE_EXPORT TDECModule *create_weatherservice( TQWidget *parent, const char * ) {
    return new KCMWeatherService( parent, "kweather" );
  }
}

KCMWeatherService::KCMWeatherService( TQWidget *parent, const char *name )
  : TDECModule( parent, name )
{
  TQVBoxLayout *layout = new TQVBoxLayout( this );
  mWidget = new ServiceConfigWidget( this );

  // not needed, as a change immediately changes the service
  //connect(mWidget, TQT_SIGNAL(changed(bool)), this, TQT_SIGNAL(changed(bool)));

  layout->addWidget( mWidget );
  TDEAboutData *about = new TDEAboutData( "kcmweatherservice",
                                      I18N_NOOP( "KWeather Configure Dialog" ),
                                      0, 0, TDEAboutData::License_GPL,
                                      I18N_NOOP( "(c), 2003 Tobias Koenig" ) );

  about->addAuthor( "Tobias Koenig", 0, "tokoe@kde.org" );
  setAboutData(about);
}

void KCMWeatherService::load()
{
}

void KCMWeatherService::save()
{
}

void KCMWeatherService::defaults()
{
}

#include "kcmweatherservice.moc"
