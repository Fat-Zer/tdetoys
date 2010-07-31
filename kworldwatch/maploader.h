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
#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <tqpixmap.h>
#include <tqbitmap.h>
#include <tqptrlist.h>


class MapTheme
{
public:

  MapTheme() : _name(""), _tag(""), _id(0) {};
  MapTheme(const TQString &name, const TQString &tag) : _name(name), _tag(tag), _id(0) {};

  TQString tag() { return _tag; };
  TQString name() { return _name; };

  void setID(int i) { _id = i; };
  int ID() { return _id; };


private:
  
  TQString _name, _tag;
  int _id;
  
};


class MapLoader
{
public:

  static TQPtrList<MapTheme> themes();

  void load(unsigned int width=400, const TQString &theme = "earth", unsigned int height=0, float opacity=0.5);

  TQPixmap lightMap() { return _light; };
  TQPixmap darkMap() { return _dark; };

  TQBitmap darkMask(int width, int height);


private:

  TQStringList maps(const TQString &theme);

  TQPixmap _light, _dark;

};


#endif
