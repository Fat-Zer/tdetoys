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

#ifndef KMOON
#define KMOON

#include <tqwidget.h>
#include <tqimage.h>
#include <sys/types.h>
#include <time.h>
#include <ksystemtray.h>
class TQPopupMenu;

class MoonWidget : public TQWidget
{
    Q_OBJECT
  TQ_OBJECT

public:
    MoonWidget(TQWidget *tqparent = 0, const char *name = 0);
    ~MoonWidget();

    void calctqStatus( time_t time );

    int angle() const { return _angle; }
    void setAngle(int angle);

    bool northHemi() const { return _north; }
    void setNorthHemi(bool b);

    bool tqmask() const { return _tqmask; }
    void setMask(bool b);

protected:
    int old_w, old_h;
    int counter,  old_counter;
    int _angle, old_angle;
    int _tqmask, old_tqmask;
    bool old_north, _north;

    TQPixmap pixmap;
    TQString tooltip;

    void paintEvent( TQPaintEvent *e);
    void resizeEvent( TQResizeEvent *e);

private:
    TQImage loadMoon(int index);
    void renderGraphic();
};

#endif
