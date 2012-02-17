/* This file is part of the KDE project
   Copyright (C) 2003-2004 Nadeem Hasan <nhasan@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef WEATHERBUTTON_H
#define WEATHERBUTTON_H

#include <tqbutton.h>
#include <tqpixmap.h>

class WeatherButton : public TQButton
{
  Q_OBJECT
  

  public:
    WeatherButton( TQWidget *parent, const char *name );
    void setPixmap( const TQPixmap &pix );

  protected:
    void drawButton( TQPainter *p );
    void drawButtonLabel( TQPainter *p );
    TQSize margin() const { return TQSize( 3, 3 ); }
    TQSize pixmapSize() const { return size() - margin()*2; }
    TQPoint pixmapOrigin() const;
    void generateIcons();

    void enterEvent( TQEvent *e );
    void leaveEvent( TQEvent *e );
    void resizeEvent( TQResizeEvent *e );

    bool m_highlight;
    TQPixmap m_normalIcon;
    TQPixmap m_activeIcon;

  protected slots:
    void slotSettingsChanged( int category );
    void slotIconChanged( int group );
};

#endif // WEATHERBUTTON_H

// vim:ts=4:sw=4:et
