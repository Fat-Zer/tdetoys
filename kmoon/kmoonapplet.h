/*
 *   kmoon - a moon phase indicator
 *   Copyright (C) 1998,2000  Stephan Kulow
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef KMOONAPPLET
#define KMOONAPPLET

#include <tqwidget.h>
#include <tqimage.h>
#include <sys/types.h>
#include <time.h>
#include <ksystemtray.h>
#include <kpanelapplet.h>

// #include "kmoonwidget.h"

class TQPopupMenu;

class MoonWidget;

class MoonPAWidget : public KPanelApplet
{
    Q_OBJECT
  

public:
    MoonPAWidget(const TQString& configFile, Type t = Normal, int actions = 0,
		 TQWidget *parent = 0, const char *name = 0);
    ~MoonPAWidget();

    int widthForHeight(int height) const { return height; }
    int heightForWidth(int width) const { return width; }

protected:
    TQPopupMenu *popup;
    MoonWidget *moon;
    TQString tooltip;

    void about() { showAbout(); }
    void preferences() { settings(); }

protected slots:

    void timerEvent( TQTimerEvent *e);
    void showAbout();
    void settings();

protected:
    void mousePressEvent( TQMouseEvent *e);
};

#endif
