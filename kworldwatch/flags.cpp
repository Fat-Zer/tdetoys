/*
**
** Copyright (C) 1998-2001 by Matthias H�lzer-Kl�pfel <hoelzer@kde.org>
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
#include <limits.h>


#include <kstandarddirs.h>


#include "flags.h"


FlagList::FlagList()
{
  _flags.setAutoDelete(true);

  _flagPixmap = TQPixmap(locate("data", "kworldclock/pics/flag.png"));
  _flagMask = TQPixmap(locate("data", "kworldclock/pics/flag-mask.xpm"), 0, TQPixmap::ThresholdDither);
  _flagMask.setMask(_flagMask.createHeuristicMask());
}


void FlagList::addFlag(Flag *f)
{
  _flags.append(f);
}


TQPoint FlagList::getPosition(double la, double lo, int w, int h, int offset)
{
  int x = (int)((double)w * (180.0 + lo) / 360.0);
  int y = (int)((double)h * (90.0 - la) / 180.0);
  x = (x + offset + w/2) % w;
 
  return TQPoint(x,y);
}
 
 
void FlagList::paint(TQPainter *p, int width, int height, int offset)
{
  p->setPen(TQt::black);
 
  TQPtrListIterator<Flag> it(_flags);
  for ( ; it.current(); ++it)
    {
      TQPoint pos = getPosition(it.current()->latitude(), it.current()->longitude(), width, height, offset);

      p->setPen(it.current()->color());
      p->setBrush(it.current()->color());

      if (width > 100)
	{
          pos -= TQPoint(5,15);

	  p->drawPixmap(pos,_flagMask);
	  p->drawPixmap(pos,_flagPixmap);
	}
      else
 	p->drawEllipse(pos.x()-1, pos.y()-1, 3, 3);
    }
}                                                                         


void FlagList::removeNearestFlag(const TQPoint &target, int w, int h, int offset)
{
  Flag *flag = 0;

  TQPoint diff;

  int dist = INT_MAX;
  TQPtrListIterator<Flag> it(_flags);
  for ( ; it.current(); ++it)
    {
      diff = getPosition(it.current()->latitude(), it.current()->longitude(), w, h, offset);
      diff -= target;
      if (diff.manhattanLength() < dist)
	{
	  dist = diff.manhattanLength();
	  flag = it.current();
	}
    }

  if (flag)
    _flags.remove(flag);
}

void FlagList::removeAllFlags()
{
  _flags.clear();
}

void FlagList::save(TDEConfig *config)
{
  config->writeEntry("Flags", _flags.count());
  
  TQPtrListIterator<Flag> it(_flags);
  int cnt=0;
  for ( ; it.current(); ++it)
    {
      config->writeEntry(TQString("Flag_%1_Color").arg(cnt), it.current()->color());
      config->writeEntry(TQString("Flag_%1_Latitude").arg(cnt), it.current()->latitude());
      config->writeEntry(TQString("Flag_%1_Longitude").arg(cnt), it.current()->longitude());
      cnt++;
    }
}


void FlagList::load(TDEConfig *config)
{
  _flags.clear(); 
  int num = config->readNumEntry("Flags", 0);

  for (int i=0; i<num; ++i)
    {
      addFlag(new Flag(config->readDoubleNumEntry(TQString("Flag_%1_Longitude").arg(i)),
		       config->readDoubleNumEntry(TQString("Flag_%1_Latitude").arg(i)),
		       config->readColorEntry(TQString("Flag_%1_Color").arg(i))));
    }
}


