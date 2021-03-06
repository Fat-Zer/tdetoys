/*
 *  Copyright (c) 2000 Matthias Elter <elter@kde.org>
 *  based on keyes (C) 1999 by Jerome Tollet <tollet@magic.fr>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 */

#include <math.h>

#include <tqpainter.h>
#include <tqcursor.h>
#include <tqimage.h>

#include <tdelocale.h>
#include <tdeglobal.h>
#include <tdeapplication.h>

#include "eyes.h"
#include "eyes.moc"

#define AAFACTOR 4

extern "C"
{
    KDE_EXPORT KPanelApplet* init(TQWidget *parent, const TQString& configFile)
    {
        TDEGlobal::locale()->insertCatalogue("keyesapplet");
        EyesApplet *applet = new EyesApplet(configFile, KPanelApplet::Normal, 0, parent, "keyesapplet");
        return applet;
    }
}

EyesApplet::EyesApplet(const TQString& configFile, Type t, int actions,
                       TQWidget *parent, const char *name)
  : KPanelApplet( configFile, t, actions, parent, name )
{
    setWFlags(WNoAutoErase);
    setBackgroundOrigin(AncestorOrigin);
    startTimer(50);
    oldleft = TQPoint(-1, -1);
    oldright = TQPoint(-1, -1);
    oldMouse = TQPoint(-1, -1);
}

int EyesApplet::widthForHeight(int h) const
{
    return static_cast<int>(1.4 * h); // rectangular shape.
}
int EyesApplet::heightForWidth(int w) const
{
    return static_cast<int>(w / 1.4); // rectangular shape.
}

void EyesApplet::resizeEvent( TQResizeEvent*e )
{
    TQWidget::resizeEvent(e);
}

void EyesApplet::timerEvent(TQTimerEvent*)
{
    TQPoint mouse = mapFromGlobal(TQCursor::pos());
    if (mouse != oldMouse)
        update();
}

void EyesApplet::paintEvent(TQPaintEvent*)
{
    int spWidth = width() * AAFACTOR;
    int spHeight = height() * AAFACTOR;
    
    if (spWidth != _cache.width() || spHeight != _cache.height())
        _cache.resize(spWidth, spHeight);
    
    TQPainter paint(&_cache);
    
    if (paletteBackgroundPixmap())
    {
        TQPixmap bg(width(), height());
        TQPainter p(&bg);
        TQPoint offset = backgroundOffset();
        p.drawTiledPixmap(0, 0, width(), height(), *paletteBackgroundPixmap(), offset.x(), offset.y());
        p.end();
        TQImage bgImage = bg.convertToImage().scale(spWidth, spHeight);
        paint.drawImage(0, 0, bgImage);
    }
    else
    {
        _cache.fill(paletteBackgroundColor());
    }
    
    // draw eyes, no pupils
    paint.setPen(TQPen(black, 2 * AAFACTOR));
    paint.setBrush(TQBrush(white));
    
    int w = spWidth; // - AAFACTOR * 2;
    int h = spHeight; // - AAFACTOR * 2;
    
    // left eye
    paint.drawEllipse(AAFACTOR, AAFACTOR, w/2 - AAFACTOR, h - AAFACTOR * 2);
    
    // right eye
    paint.drawEllipse(w/2, AAFACTOR, w/2 - AAFACTOR, h - AAFACTOR * 2);
    
    // draw pupils
    drawPupils(&paint);
    paint.end();
    
    TQPainter paintFinal(this);
    TQImage spImage = _cache.convertToImage();
    TQImage displayImage = spImage.smoothScale(size());
    paintFinal.drawImage(0, 0, displayImage);
    paintFinal.end();
}

void EyesApplet::drawPupils(TQPainter* p)
{
    TQPoint pos, mouse, vect;
    double cos_alpha,sin_alpha;
    
    int w = width() * AAFACTOR;
    int h = height() * AAFACTOR;

    oldMouse = mapFromGlobal(TQCursor::pos());
    mouse =  oldMouse * AAFACTOR;
    int tmp = TQMIN(h, w)/6;

    // left pupil
    vect.setX(mouse.x() - h / 4);
    vect.setY(mouse.y() - h / 2);

    cos_alpha = vect.x() / sqrt(double(vect.x() * vect.x() + vect.y() * vect.y()));
    sin_alpha = vect.y() / sqrt(double(vect.x() * vect.x() + vect.y() * vect.y()));

    if(vect.x() * vect.x() + vect.y() * vect.y() > (w/4 - tmp) * (w/4 - tmp)*
       cos_alpha * cos_alpha+ (h/2-tmp) * (h/2-tmp) * sin_alpha * sin_alpha) {
        pos.setX(int((w/4-tmp) * cos_alpha+w/4));
        pos.setY(int((h/2-tmp) * sin_alpha+h/2));
    }
    else
    	pos = mouse;

    if(pos != oldleft) {

        int sizeEye=TQMIN(h,w)/6;

//         // draw over old pos
// 	p->setPen(TQPen(NoPen));
// 	p->setBrush(TQBrush(white));
// 	p->drawEllipse(oldleft.x() - sizeEye/2, oldleft.y() - sizeEye/2, sizeEye, sizeEye);

        // draw left pupil
        p->setPen(TQPen(NoPen));
        p->setBrush(TQBrush(black));
        p->drawEllipse(pos.x() - sizeEye/2, pos.y() - sizeEye/2, sizeEye, sizeEye);

    //oldleft = pos;
    }

    // right pupil
    vect.setX(mouse.x() - 3*w/4);
    vect.setY(mouse.y() - h/2);

    cos_alpha = vect.x()/sqrt(double(vect.x()*vect.x()+vect.y()*vect.y()));
    sin_alpha = vect.y()/sqrt(double(vect.x()*vect.x()+vect.y()*vect.y()));

    if(vect.x()*vect.x() + vect.y()*vect.y() > (w/4-tmp)*(w/4-tmp)
       *cos_alpha*cos_alpha+(h/2-tmp)*(h/2-tmp)*sin_alpha*sin_alpha)
    {
        pos.setX(int((w/4-tmp)*cos_alpha+3*w/4));
        pos.setY(int((h/2-tmp)*sin_alpha+h/2));
    }
    else
        pos = mouse;

    if(pos != oldright) {

        int sizeEye=TQMIN(h,w)/6;

//         // draw over old pos
// 	p->setPen(TQPen(NoPen));
// 	p->setBrush(TQBrush(white));
// 	p->drawEllipse(oldright.x() - sizeEye/2, oldright.y() - sizeEye/2, sizeEye, sizeEye);

        // draw left pupil
        p->setPen(TQPen(NoPen));
        p->setBrush(TQBrush(black));
        p->drawEllipse(pos.x() - sizeEye/2, pos.y() - sizeEye/2, sizeEye, sizeEye);

    //oldright = pos;
    }
}
