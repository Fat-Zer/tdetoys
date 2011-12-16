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
#include <time.h>

#include <tqvaluelist.h>
#include <tqdir.h>
#include <tqimage.h>
#include <tqpainter.h>
#include <tqtl.h>
#include <tqstringlist.h>


#include <kglobal.h>
#include <kstandarddirs.h>
#include <kdesktopfile.h>
#include <kimageeffect.h>


#include "astro.h"
#include "maploader.h"


TQPtrList<MapTheme> MapLoader::themes()
{
  TQPtrList<MapTheme> result;

  TQStringList files = KGlobal::dirs()->findAllResources("data", "kworldclock/maps/*/*.desktop");
  for (TQStringList::Iterator it=files.begin(); it != files.end(); ++it)
    {
      KDesktopFile conf(*it);
      conf.setGroup("Theme");
      result.append(new MapTheme(conf.readName(), conf.readEntry("Theme")));
    }

  return result;
}


TQStringList MapLoader::maps(const TQString &theme)
{
  return KGlobal::dirs()->findAllResources("data", TQString("kworldclock/maps/%1/*.jpg").tqarg(theme));
}


void MapLoader::load(unsigned int width, const TQString &theme, unsigned int height, float opacity)
{
  // find the maps available
  TQValueList<uint> sizes;
  TQStringList files = maps(theme);
  for (uint i=0; i<files.count(); ++i)
    {
      TQString f = files[i];
      int pos = f.findRev("/");
      if (pos >= 0)
        f = f.mid(pos+1);
      pos = f.findRev(".");
      if (pos >= 0)
        f = f.left(pos);
      sizes.append(f.toInt());
    }
  qHeapSort(sizes);

  // find the closest (bigger) size
  uint size=0;
  for (uint i=0; i<sizes.count(); ++i)
    {
      size = sizes[i];
      if (size >= width)
	break;
    }
  
  TQImage image;
  if (size == 0)
    {
      image = TQImage(locate("data", "kworldclock/maps/depths/800.jpg"));
      size = 800;
    }
  else  
    image = TQImage(locate("data", TQString("kworldclock/maps/%1/%2.jpg").tqarg(theme).tqarg(size)));

  if (height == 0)
    height = width/2;

  if ((image.width() != (int)width) || (image.height() != (int)height))
    image = image.smoothScale(width, height);

  // convert to light map
  _light.convertFromImage(image);

  // calculate dark map
  _dark.convertFromImage(KImageEffect::blend(TQt::black, image, opacity));
}


TQBitmap MapLoader::darkMask(int width, int height)
{
  time_t t;
  struct tm *tmp;
  double jt, sunra, sundec, sunrv, sunlong;
  short *wtab;

  TQBitmap illuMask(width, height);
 
  // calculate the position of the sun
  t = time(NULL);
  tmp = gmtime(&t);
  jt = jtime(tmp);
  sunpos(jt,FALSE, &sunra, &sundec, &sunrv, &sunlong);

  int sec = tmp->tm_hour*60*60 + tmp->tm_min*60 + tmp->tm_sec;
  int gmt_position = width * sec / 86400; // note: greenwich is in the middle!

  // calculate the illuminated area
  wtab = new short[height];
  projillum(wtab,width,height,sundec);
 
  // draw illumination
  illuMask.fill(TQt::black);
  TQPainter p;
  p.begin(&illuMask);
 
  int start, stop;
  int middle = width - gmt_position;
  for (int y=0; y<height; y++)
    if (wtab[y]>0)
      {
	start = middle - wtab[y];
	stop = middle + wtab[y];
	if (start < 0)
	  {
	    p.drawLine(0,y,stop,y);
	    p.drawLine(width+start,y,width,y);
	  }
	else
	  if (stop > width)
	    {
	      p.drawLine(start,y,width,y);
	      p.drawLine(0,y,stop-width,y);
	    }
	  else
	    p.drawLine(start,y,stop,y);
      }
  p.end();
  delete [] wtab;
  return illuMask;
}

