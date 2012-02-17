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
#ifndef ZONECLOCK_H
#define ZONECLOCK_H


#include <tqwidget.h>
#include <tqstring.h>
#include <tqframe.h>
#include <tqptrlist.h>


class TQLabel;
class SimpleFlow;
class KConfig;


class ClockDialog;


class ZoneClock : public TQFrame
{
  Q_OBJECT
  

public:

  ZoneClock(const TQString &zone, const TQString &name, TQWidget *parent=0, const char *n=0);


  TQString zone() const { return _zone; };
  void setZone(const TQString &z) { _zone = z; updateTime(); };

  TQString name() const { return _name; };
  void setName( const TQString &n) { _name = n; updateTime(); };


signals:

  void removeMe(ZoneClock *t);
  void addClock(const TQString &zone);
  void changed();


public slots:

  void updateTime();


protected:

  virtual bool eventFilter(TQObject *, TQEvent *);


private slots:

  void editClock();
  void slotRemoveClock();
  void removeTimeout();
  void slotAddClock();


private:

  TQString _zone;
  TQString _name;
  TQLabel  *_timeLabel, *_nameLabel;
  TQPopupMenu *_popup;

};


class ZoneClockPanel : public TQFrame
{
  Q_OBJECT
  

public:

  ZoneClockPanel(TQWidget *parent=0, const char *name=0);

  void addClock(const TQString &zone, const TQString &name);

  void save(KConfig *config);
  void load(KConfig *config);


public slots:

  void addClock(const TQString &zone);


private slots:

  void updateTimer();
  void realign();
  void removeClock(ZoneClock *);


private:

  void createDialog();

  SimpleFlow *_flow;
  TQPtrList<ZoneClock> _clocks;
  ClockDialog *_dlg;

};


#endif
