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
#ifndef FLAGS_H
#define FLAGS_H


#include <tqstring.h>
#include <tqcolor.h>
#include <tqptrlist.h>
#include <tqpoint.h>
#include <tqpainter.h>
#include <tqpixmap.h>
#include <tqbitmap.h>


#include <tdeconfig.h>


class Flag
{
public:
  
  Flag(double lo, double la, const TQColor &col)
    : _lo(lo), _la(la), _col(col) {};

  double longitude() const { return _lo; };
  double latitude() const { return _la; };

  TQColor color() const { return _col; };

  TQString annotation() const { return _ann; };
  void setAnnotation(const TQString &ann) { _ann = ann; };


private:

  double _lo, _la;

  TQColor _col;

  TQString _ann;

};


class FlagList
{
public:

  FlagList();

  void paint(TQPainter *p, int w, int h, int offset);

  void addFlag(Flag *f);

  void removeNearestFlag(const TQPoint &target, int w, int h, int offset);
  void removeAllFlags();

  void save(TDEConfig *config);
  void load(TDEConfig *config);
  

private:
 
  TQPoint getPosition(double la, double lo, int w, int h, int offset); 

  TQPtrList<Flag> _flags;

  TQPixmap _flagPixmap;
  TQBitmap _flagMask;

};


#endif
