/***************************************************************************
                          dockwidget.h  -  description
                             -------------------
    begin                : Thu Jul 6 2000
    copyright            : (C) 2000-2003 by Ian Reinhart Geiser
                         : (C) 2002-2003 Nadeem Hasan <nhasan@kde.org>
    email                : geiseri@msoe.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <tqfont.h>
#include <tqpixmap.h>
#include <tqwidget.h>

#include <dcopref.h>

class TQLabel;
class WeatherService_stub;
class WeatherButton;

class dockwidget : public QWidget
{
    Q_OBJECT

public:
    dockwidget(const TQString &location, TQWidget *parent=0, const char *name=0);
    ~dockwidget();

    enum {ShowIconOnly=1, ShowTempOnly=2, ShowAll=3 };

    void setLocationCode(const TQString &locationCode);
    void setViewMode(int);
    void setOrientation(Orientation o) { m_orientation = o; }
    /** resize the view **/
    void resizeView(const TQSize &size);
    int widthForHeight(int h);
    int heightForWidth(int w);

public  slots:
    void showWeather();

signals: // Signals
    void buttonClicked();

private:
    void initDock();
    void updateFont();

    int m_mode;
    TQString m_locationCode;
    TQFont m_font;
    TQPixmap m_icon;
    WeatherButton *m_button;
    TQLabel *m_lblTemp;
    TQLabel *m_lblWind;
    TQLabel *m_lblPres;
    Orientation m_orientation;

    WeatherService_stub *m_weatherService;
};

#endif

// vim:ts=4:sw=4:et
