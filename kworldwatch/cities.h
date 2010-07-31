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
#ifndef CITIES_H
#define CITIES_H


#include <tqstring.h>
#include <tqstringlist.h>
#include <tqptrlist.h>


class QPainter;


class City
{
public:

  City(const TQString &n, double la, double lo) : _name(n), _latitude(la), _longitude(lo) {};
  TQString name() { return _name; };

  double latitude() { return _latitude; };
  double longitude() { return _longitude; };


private:

  TQString _name;
  double _latitude, _longitude;

};


class CityList
{
public:

  CityList();
  ~CityList();
  void paint(TQPainter *p, int width, int height, int offset);
  
  City *getNearestCity(int w, int h, int offset, int x, int y, TQPoint &where);

  TQStringList timezones();

  
private:

  void readCityLists();
  void readCityList(const TQString &fname);

  TQPoint getPosition(double la, double lo, int w, int h, int offset);


private:

  TQPtrList<City> _cities;

};


#endif
