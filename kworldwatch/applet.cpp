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
#include <tqlayout.h>


#include <tdeapplication.h>
#include <tdeglobal.h>
#include <tdelocale.h>
#include <kiconloader.h>
#include <krun.h>
#include <tdeglobalsettings.h>


#include "mapwidget.h"
#include "applet.moc"


extern "C"
{
  KDE_EXPORT KPanelApplet *init(TQWidget *parent, const TQString& configFile)
  {
    TDEGlobal::locale()->insertCatalogue("kworldclock");
    TDEGlobal::locale()->insertCatalogue("timezones"); // For time zone translation
    return new KWWApplet(configFile, KPanelApplet::Normal,
			 0,
			 parent, "kwwapplet");
  }
}


KWWApplet::KWWApplet(const TQString& configFile, Type type, int actions,
		     TQWidget *parent, const char *name)
  : KPanelApplet(configFile, type, actions, parent, name)
{
  // make use of the icons installed for ksaferppp
  TDEGlobal::iconLoader()->addAppDir("kworldwatch");

  TQVBoxLayout *vbox = new TQVBoxLayout(this, 0,0);

  map = new MapWidget(true, true, this);
  map->load(config());
  vbox->addWidget(map);
  setCustomMenu(map->contextMenu());

  map->installEventFilter(this);
}


KWWApplet::~KWWApplet()
{
  map->save(config());
}


int KWWApplet::widthForHeight(int height) const
{
  return height*2;
}


int KWWApplet::heightForWidth(int width) const
{
  return width/2;
}


// catch the mouse clicks of our child widgets
bool KWWApplet::eventFilter( TQObject *o, TQEvent *e )
{
  if ((e->type() == TQEvent::MouseButtonPress) || (e->type() == TQEvent::MouseButtonDblClick)) 
  {
    mousePressEvent(TQT_TQMOUSEEVENT(e));
    return true;
  }

  return KPanelApplet::eventFilter(o, e);
}


void KWWApplet::mousePressEvent(TQMouseEvent *e)
{
  bool clicked = false;

  if (TDEGlobalSettings::singleClick())
  {
    clicked = e->type() == TQMouseEvent::MouseButtonPress;
  }
  else
  {
    clicked = e->type() == TQMouseEvent::MouseButtonDblClick;
  }

  if (clicked && e->button() == Qt::LeftButton) 
  {
    KRun::run("kworldclock", KURL::List());
  }
}
