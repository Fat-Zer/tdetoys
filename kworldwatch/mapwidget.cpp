/*
**
** Copyright (C) 1998-2001 by Matthias Hözer-Klüpfel <hoelzer@kde.org>
**	Maintainence has ceased - send questions to kde-devel@kde.org.
**
*/

/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program in a file called COPYING; if not, write to
** the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
** MA 02110-1301, USA.
*/

/*
** Bug reports and questions can be sent to kde-devel@kde.org
*/
#include "config.h"

#include <time.h>
#include <stdlib.h>

#include <tqdatetime.h>

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kcolordialog.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <tqcursor.h>
#include <tqpainter.h>
#include <tqpopupmenu.h>
#include <tqregexp.h>
#include <tqiconset.h>
#include <tqtooltip.h>


#include "cities.h"
#include "about.h"
#include "flags.h"
#include "mapwidget.moc"


MapWidget::MapWidget(bool applet, bool restore, TQWidget *tqparent, const char *name)
  : TQWidget(tqparent, name), _loader(), _illumination(true), _cities(true), _flags(true), _cityList(0),
    _applet(applet), _width(0), _height(0)
{
  // this ugly construction is necessary so we don't load 
  // the map twice.
  _theme = "depths";

  if (restore)
    {
      KConfig *config = kapp->config();
      if (applet)
	config = new KConfig("kwwwappletrc");
      _theme = config->readEntry("Theme", "depths");
      if (applet)
	delete config;
    }

  setBackgroundMode(TQWidget::NoBackground);
  
  gmt_position = 0;
  time_t t = time(NULL);
  setTime(gmtime(&t));

  _flagList = new FlagList;

  int id;
  _flagPopup = new TQPopupMenu(this);
  TQPixmap flag = TQPixmap(locate("data", "kworldclock/pics/flag-red.png"));
  id = _flagPopup->insertItem(TQIconSet(flag), i18n("Add &Red"), this, TQT_SLOT(addFlag(int)));
  _flagPopup->setItemParameter(id, 0);
  flag = TQPixmap(locate("data", "kworldclock/pics/flag-green.png"));
  id = _flagPopup->insertItem(TQIconSet(flag), i18n("Add &Green"), this, TQT_SLOT(addFlag(int)));
  _flagPopup->setItemParameter(id, 1);
  flag = TQPixmap(locate("data", "kworldclock/pics/flag-blue.png"));
  id = _flagPopup->insertItem(TQIconSet(flag), i18n("Add &Blue"), this, TQT_SLOT(addFlag(int)));
  _flagPopup->setItemParameter(id, 2);
  id = _flagPopup->insertItem(i18n("Add &Custom..."), this, TQT_SLOT(addFlag(int)));
  _flagPopup->setItemParameter(id, 3);
  _flagPopup->insertSeparator();
  _flagPopup->insertItem(i18n("&Remove Flag"), this, TQT_SLOT(removeFlag()));
  _flagPopup->insertItem(i18n("&Remove All Flags"), this, TQT_SLOT(removeAllFlags()));

  _themePopup = new TQPopupMenu(this);
  _themes = MapLoader::themes();
  int cnt=0;
  TQPtrListIterator<MapTheme> it(_themes);
  for ( ; it.current(); ++it)
    {
      int id = _themePopup->insertItem(it.current()->name(), this, TQT_SLOT(themeSelected(int)));
      _themePopup->setItemParameter(id, cnt++);
      it.current()->setID(id);
    }

  TQPopupMenu *_clocksPopup = new TQPopupMenu(this);
  _clocksPopup->insertItem(i18n("&Add..."), this, TQT_SLOT(addClock()));

  _popup = new TQPopupMenu(this);
  _popup->insertItem(i18n("&Flags"), _flagPopup);
  
  if (!applet)
    _popup->insertItem(i18n("&Clocks"), _clocksPopup);

  _popup->insertSeparator();
  _popup->insertItem(i18n("&Map Theme"), _themePopup);
  _illuminationID = _popup->insertItem(i18n("Show &Daylight"), this, TQT_SLOT(toggleIllumination()));
  _citiesID = _popup->insertItem(i18n("Show &Cities"), this, TQT_SLOT(toggleCities()));
  _flagsID = _popup->insertItem(i18n("Show F&lags"), this, TQT_SLOT(toggleFlags()));

  if (!applet)
    {
      _popup->insertSeparator();
      _popup->insertItem(i18n("&Save Settings"), this, TQT_SLOT(slotSaveSettings()));
    }

  _popup->insertSeparator();
  _popup->insertItem(i18n("&About"), this, TQT_SLOT(about()));

  TQTimer *timer = new TQTimer(this);
  connect(timer, TQT_SIGNAL(timeout()), this, TQT_SLOT(timeout()));
  timer->start(1000);

  _cityIndicator = new TQLabel(0,0, WStyle_StaysOnTop | WStyle_Customize | WStyle_NoBorder | WStyle_Tool | WX11BypassWM );
  _cityIndicator->setMargin(1);
  _cityIndicator->setIndent(0);
  _cityIndicator->setAutoMask(false);
  _cityIndicator->setLineWidth(1);
  _cityIndicator->tqsetAlignment(TQLabel::AlignAuto | TQLabel::AlignTop);
  _cityIndicator->setAutoResize(true);
  _cityIndicator->setFrameStyle(TQFrame::Box | TQFrame::Plain);
  _cityIndicator->setPalette(TQToolTip::palette());

  if (restore && !applet)
    load(kapp->config());
    
  connect(&m_timer, TQT_SIGNAL(timeout()), this, TQT_SLOT(updateCityIndicator()));
}


MapWidget::~MapWidget()
{
  if (_applet)
    {
      KConfig *conf = new KConfig("kwwwappletrc");
      save(conf);
      delete conf;
    }
  delete _cityList;
  delete _flagList;
}


void MapWidget::load(KConfig *config)
{
  setCities(config->readBoolEntry("Cities", true));
  setIllumination(config->readBoolEntry("Illumination", true));
  setFlags(config->readBoolEntry("Flags", true));

  if ( !_applet )
  {
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->isSet("theme"))
      setTheme(TQString::fromLocal8Bit(args->getOption("theme")));
    else
      setTheme(config->readEntry("Theme", "depths"));

    _flagList->load(config);  
  }
}


void MapWidget::save(KConfig *config)
{
  config->writeEntry("Cities", _cities);
  config->writeEntry("Illumination", _illumination);
  config->writeEntry("Flags", _flags);
  
  config->writeEntry("Theme", _theme);

  _flagList->save(config);
}


void MapWidget::slotSaveSettings()
{
  emit saveSettings();
}


void MapWidget::addClock()
{
  if (!_cityList)
    _cityList = new CityList;

  TQPoint where;
  City *c = _cityList->getNearestCity(_width, _height, gmt_position, _flagPos.x(), _flagPos.y(), where);     
  
  TQString zone = "";
  if (c)
    zone = c->name();

  emit addClockClicked(zone);
}


void MapWidget::addFlag(int index)
{
  TQColor col = TQt::red;

  switch (index)
    {
    case 0:
      col = TQt::red;
      break;
    case 1:
      col = TQt::green;
      break;
    case 2:
      col = TQt::blue;
      break;
    case 3:
      if (KColorDialog::getColor(col, this) != KColorDialog::Accepted)
	return; 
      break;
    }

  int x = _flagPos.x() - gmt_position + width()/2;
  if (x>width())
    x -= width();
  double la = 90.0 - 180.0 * ((double)_flagPos.y()) / ((double)_height);
  double lo = 360.0 * ((double)x) / ((double)_width) - 180.0;

  _flagList->addFlag(new Flag(lo, la, col));

  update();
}


void MapWidget::removeFlag()
{
  _flagList->removeNearestFlag(_flagPos, _width, _height, gmt_position);
  update();
}

void MapWidget::removeAllFlags()
{
  if ( KMessageBox::warningContinueCancel( this, i18n( "Do you really want to remove all flags?" ), TQString(), KStdGuiItem::del() ) == KMessageBox::Continue )
    _flagList->removeAllFlags();

  update();
}

void MapWidget::setCities(bool c)
{
  _cities = c;
  _popup->setItemChecked(_citiesID, c);

  if (c && !_cityList)
    _cityList = new CityList;

  setMouseTracking(c);
  if (!c)
  {
    _cityIndicator->hide();
    m_timer.stop();
  }
    
  update();  
}


void MapWidget::toggleCities()
{
  setCities(!_popup->isItemChecked(_citiesID));
}


void MapWidget::toggleIllumination()
{
  setIllumination(!_popup->isItemChecked(_illuminationID));
}


void MapWidget::setFlags(bool f)
{
  _flags = f;
  _popup->setItemChecked(_flagsID, f);

  update();
}


void MapWidget::toggleFlags()
{
  setFlags(!_popup->isItemChecked(_flagsID));
}


void MapWidget::updateBackground()
{
  if (!_height)
    return;
  _pixmap = calculatePixmap();
  setBackgroundPixmap(_pixmap);

  update();
}


TQPixmap MapWidget::getPixmap()
{
  return _pixmap;
}


void MapWidget::setIllumination(bool i)
{
  _illumination = i;
  _popup->setItemChecked(_illuminationID, i);

  updateBackground();
}


void MapWidget::timeout()
{
  time_t t = time(NULL);
  setTime(gmtime(&t));

  if (_cities && !_currentCity.isEmpty())
    _cityIndicator->setText(cityTime(_currentCity));
}


TQString MapWidget::cityTime(const TQString &city)
{
  TQString result = i18n(city.latin1()); // Time zone translation
  int pos = result.find("/");
  if (pos >= 0)
    result = result.mid(pos+1);
  result.replace(TQRegExp("_"), " ");
  result.append(": ");

  char *initial_TZ = getenv("TZ");
  setenv("TZ", city.latin1(), 1);
  tzset();
  
  time_t t = time(NULL);
  TQDateTime dt;
  dt.setTime_t(t);
  result.append(TQString("%1, %2").tqarg(KGlobal::locale()->formatTime(dt.time(), true)).tqarg(KGlobal::locale()->formatDate(dt.date(), true)));

  if (initial_TZ != 0)
    setenv("TZ", initial_TZ, 1);
  else
    unsetenv("TZ");
  tzset(); 
  return result;
}


void MapWidget::enterEvent(TQEvent *)
{
  if ( _cities )
    updateCityIndicator();
} 
 
void MapWidget::leaveEvent(TQEvent *)
{
  _cityIndicator->hide();
  m_timer.stop();
}


void MapWidget::about()
{
  AboutDialog dlg(this, 0, true);
  dlg.exec();
}


void MapWidget::themeSelected(int index)
{
  TQString t = _themes.at(index)->tag();
  if (!t.isEmpty())
    setTheme(t);
}


void MapWidget::mousePressEvent(TQMouseEvent *ev)
{
  if (ev->button() == Qt::RightButton)
    {
      _flagPos = ev->pos();
      _popup->exec(ev->globalPos()); 
    }
} 


void MapWidget::mouseMoveEvent(TQMouseEvent *)
{
  if (!_cities)
    return;

  if (!m_timer.isActive())
     m_timer.start(25, true);
}

void MapWidget::updateCityIndicator()
{
  TQPoint where;
  TQPoint pos = mapFromGlobal(TQCursor::pos());
  
  if (!_cityList)
    _cityList = new CityList;
  City *c = _cityList->getNearestCity(_width, _height, gmt_position, pos.x(), pos.y(), where);

  if (c)
    {
      _currentCity = c->name();
      showIndicator(TQCursor::pos());
    }
  else
    _cityIndicator->hide();
}


void MapWidget::showIndicator(const TQPoint &pos)
{
  _cityIndicator->setText(cityTime(_currentCity));

  int w = _cityIndicator->width();
  int h = _cityIndicator->height();

  TQRect desk = KGlobalSettings::desktopGeometry(pos);

	TQPoint newPos;

  if (pos.x()+w+10 > desk.right())
    newPos.setX(pos.x()-w-5);
  else 
    newPos.setX(pos.x()+10);

  if (pos.y()+h+10 > desk.bottom())
    newPos.setY(pos.y()-h-5);
  else
    newPos.setY(pos.y()+10);
      
  _cityIndicator->move(newPos);

  _cityIndicator->show();

}


void MapWidget::setTheme(const TQString &theme)
{
  _theme = theme;

  TQPtrListIterator<MapTheme> it(_themes);
  for ( ; it.current(); ++it)
   _themePopup->setItemChecked(it.current()->ID(), theme == it.current()->tag());

  if (_height)
    setSize(_width, _height);
}


void MapWidget::setTime(struct tm *time)
{
  sec = time->tm_hour*60*60 + time->tm_min*60 + time->tm_sec;
  
  if (isVisible())
  {
     int old_position = gmt_position;
     gmt_position = width() * sec / 86400; // note: greenwich is in the middle!
  
     if (old_position != gmt_position)
        updateBackground();
  }
}

void MapWidget::setSize(int w, int h)
{
  _width = w;
  _height = h;
  _loader.load(_width, _theme, _height);

  gmt_position = width() * sec / 86400; // note: greenwich is in the middle!

  updateBackground();
}

void MapWidget::resizeEvent(TQResizeEvent *ev)
{
  setSize(width(), height());
  TQWidget::resizeEvent(ev);
}

void MapWidget::paintContents(TQPainter *p)
{
  if (_cities)
    _cityList->paint(p, _width, _height, gmt_position);
  if (_flags)
    _flagList->paint(p, _width, _height, gmt_position);
}

void MapWidget::paintEvent(TQPaintEvent *ev)
{
  TQWidget::paintEvent(ev);

  if (_cities || _flags)
    {
       TQPainter p(this);

       p.setClipping(true);
       p.setClipRegion(ev->region());
       
       paintContents(&p);
    }
}


TQPixmap MapWidget::calculatePixmap()
{
  TQPixmap map;

  if (_illumination)
    {
      map = _loader.darkMap();
      TQPixmap clean = _loader.lightMap();

      TQPainter mp(&map);
      clean.setMask(_loader.darkMask(map.width(), map.height()));
      mp.drawPixmap(0,0, clean);
    }
  else
    map = _loader.lightMap();

  int greenwich = map.width()/2;

  TQPixmap pm(_width, _height);
  TQPainter p;
  p.begin(&pm);

  if (gmt_position >= greenwich)
    {
      p.tqdrawPixmap(gmt_position-greenwich, 0, map, 0, 0, map.width()-gmt_position+greenwich);
      p.tqdrawPixmap(0,0, map, map.width()-gmt_position+greenwich, 0, gmt_position-greenwich);
    }
  else
    {
      p.tqdrawPixmap(0,0, map, greenwich-gmt_position, 0, map.width()+gmt_position-greenwich);
      p.tqdrawPixmap(map.width()+gmt_position-greenwich, 0, map, 0, 0, greenwich-gmt_position);
    }

  return pm;
}
