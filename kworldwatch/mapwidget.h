/*
**
** Copyright (C) 1998-2001 by Matthias Hölzer-Klüpfel <hoelzer@kde.org>
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
#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H


#include <time.h>


#include <tqwidget.h>
#include <tqpixmap.h>
#include <tqstringlist.h>
#include <tqlabel.h>
#include <tqpoint.h>
#include <tqtimer.h>


class TQPopupMenu;


#include <kconfig.h>


class CityList;
class FlagList;


#include "maploader.h"


class MapWidget : public TQWidget
{
  Q_OBJECT
  TQ_OBJECT

public:

  MapWidget(bool applet=false, bool restore=false, TQWidget *parent=0, const char *name=0);
  ~MapWidget();

  void setTheme(const TQString &theme);
  void setTime(struct tm *time);
  void setIllumination(bool i);
  void setCities(bool c);
  void setFlags(bool f);
  void setSize(int w, int h);

  void save(KConfig *config);
  void load(KConfig *config);

  void updateBackground();

  TQPixmap getPixmap();
  TQPopupMenu* contextMenu() const { return _popup; }

  void paintContents(TQPainter *p);
  TQPixmap calculatePixmap();

signals:

  void addClockClicked(const TQString &zone);
  void saveSettings();


protected slots:

  void timeout();
  void updateCityIndicator();

public slots:

  void about();

  void toggleIllumination();
  void toggleCities();
  void toggleFlags();

  void removeFlag();
  void removeAllFlags();

  void slotSaveSettings();


protected:

  void resizeEvent(TQResizeEvent *ev);
  void paintEvent(TQPaintEvent *ev);
  void mousePressEvent(TQMouseEvent *ev);
  void mouseMoveEvent(TQMouseEvent *ev);
  void enterEvent(TQEvent *ev);
  void leaveEvent(TQEvent *ev);


private slots:

  void themeSelected(int index);
  void addFlag(int index);
  void addClock();


private:

  void updateMap();
  TQString cityTime(const TQString &city);
  void showIndicator(const TQPoint &pos);

  MapLoader _loader;

  TQString _theme;

  TQPixmap _pixmap;

  int gmt_position;

  time_t sec;

  TQPopupMenu *_popup, *_themePopup, *_flagPopup;
  TQPtrList<MapTheme> _themes;

  bool _illumination, _cities, _flags;
  int _illuminationID, _citiesID, _flagsID;

  CityList *_cityList;
  TQLabel *_cityIndicator;
  TQString _currentCity;

  FlagList *_flagList;
  TQPoint _flagPos;

  bool _applet;
  TQTimer m_timer;
  
  int _width, _height;
};


#endif
