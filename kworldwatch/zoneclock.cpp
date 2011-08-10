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
#include "config.h"

#include <stdlib.h>
#include <time.h>


#include <tqlabel.h>
#include <tqlayout.h>
#include <tqdatetime.h>
#include <tqtimer.h>
#include <tqcombobox.h>
#include <tqlineedit.h>
#include <tqpopupmenu.h>


#include <kglobal.h>
#include <klocale.h>
#include <kconfig.h>


#include "flow.h"
#include "clock.h"
#include "cities.h"
#include "zoneclock.moc"
#include <kdebug.h>

ZoneClock::ZoneClock(const TQString &zone, const TQString &name, TQWidget *parent, const char *n)
  : TQFrame(parent, n), _zone(zone), _name(name)
{
  setFrameStyle(TQFrame::Panel | TQFrame::Raised);
  TQHBoxLayout *hbox = new TQHBoxLayout(this, 2,2);

  _name.append(":");
  _nameLabel = new TQLabel(_name, this);
  hbox->addWidget(_nameLabel, 1);
  hbox->addSpacing(4);

  _timeLabel = new TQLabel(this);
  hbox->addWidget(_timeLabel, 0, TQt::AlignRight);

  _popup = new TQPopupMenu(this);
  _popup->insertItem(i18n("&Edit..."), this, TQT_SLOT(editClock()));
  _popup->insertItem(i18n("&Add..."), this, TQT_SLOT(slotAddClock()));
  _popup->insertItem(i18n("&Remove"), this, TQT_SLOT(slotRemoveClock()));

  _nameLabel->installEventFilter(this);
  _timeLabel->installEventFilter(this);

  updateTime();
}


void ZoneClock::slotRemoveClock()
{
  // Note: this is stupid, but we can't get us deleted
  // from this slot, as we would return and crash.
  // So instead we fire up an idle event triggering the delete
  // after the return.

  TQTimer::singleShot(0, this, TQT_SLOT(removeTimeout()));
}


void ZoneClock::removeTimeout()
{
  emit removeMe(this);
}


void ZoneClock::slotAddClock()
{
  emit addClock(_zone);
}


void ZoneClock::editClock()
{
  ClockDialog *_dlg = new ClockDialog(this, 0, true);
  CityList cities;
  TQStringList timezones = cities.timezones();
  for (TQStringList::iterator it = timezones.begin(); it != timezones.end(); ++it)
    _dlg->ClockZone->insertItem(i18n((*it).utf8()));

  _dlg->ClockCaption->setText(_nameLabel->text().left(_nameLabel->text().length()-1));
  for (int i=0; i<_dlg->ClockZone->count(); ++i)
    if (_dlg->ClockZone->text(i) == i18n(_zone.utf8()))
      {
        _dlg->ClockZone->setCurrentItem(i);
        break;
      }

  if (_dlg->exec() == TQDialog::Accepted)
    {
      _zone = timezones[_dlg->ClockZone->currentItem()];
      _name = _dlg->ClockCaption->text().append(":");
      _nameLabel->setText(_dlg->ClockCaption->text().append(":"));
      updateTime();
      tqlayout()->tqinvalidate();
      emit changed();
    }

  delete _dlg;
}


bool ZoneClock::eventFilter(TQObject *obj, TQEvent *ev)
{
  if (ev->type() == TQEvent::MouseButtonPress)
    {
      TQMouseEvent *e = (TQMouseEvent*)ev;
      if (e->button() == Qt::RightButton)
	_popup->exec(e->globalPos());
    }

  return TQFrame::eventFilter(obj, ev);
}


void ZoneClock::updateTime()
{
  char *initial_TZ = getenv("TZ");
  setenv("TZ", _zone.latin1(), 1);
  tzset();

  time_t t = time(NULL);
  TQDateTime dt;
  dt.setTime_t(t);
  _timeLabel->setText(TQString("%1, %2").tqarg(KGlobal::locale()->formatTime(dt.time(), true)).tqarg(KGlobal::locale()->formatDate(dt.date(), true)));

  if (initial_TZ != 0)
    setenv("TZ", initial_TZ, 1);
  else
    unsetenv("TZ");
  tzset();
}


ZoneClockPanel::ZoneClockPanel(TQWidget *parent, const char *name)
  : TQFrame(parent, name), _dlg(0)
{
  _flow = new SimpleFlow(this,1,1);

  TQTimer *t = new TQTimer(this);

  connect(t, TQT_SIGNAL(timeout()), this, TQT_SLOT(updateTimer()));
  t->start(500);

  _clocks.setAutoDelete(true);
}


void ZoneClockPanel::createDialog()
{
  if (!_dlg)
    {
      _dlg = new ClockDialog(this, 0, true);
      CityList cities;
      TQStringList timezones = cities.timezones();
      for (TQStringList::iterator it = timezones.begin(); it != timezones.end(); ++it)
        _dlg->ClockZone->insertItem(i18n((*it).utf8()));
    }
}


void ZoneClockPanel::addClock(const TQString &zone, const TQString &name)
{
  // add the clocks
  ZoneClock *zc = new ZoneClock(zone, name, this);
  _flow->add(zc);
  _clocks.append(zc);
  zc->show();

  realign();

  connect(zc, TQT_SIGNAL(addClock(const TQString &)), this, TQT_SLOT(addClock(const TQString &)));
  connect(zc, TQT_SIGNAL(changed()), this, TQT_SLOT(realign()));
  connect(zc, TQT_SIGNAL(removeMe(ZoneClock *)), this, TQT_SLOT(removeClock(ZoneClock *)));
}


void ZoneClockPanel::removeClock(ZoneClock *clock)
{
  _clocks.remove(clock);
  realign();
}


void ZoneClockPanel::realign()
{
  // realign the labels
  int w = 0;
  TQPtrListIterator<ZoneClock> it(_clocks);
  for ( ; it.current(); ++it)
    if (it.current()->tqsizeHint().width() > w)
      w = it.current()->tqsizeHint().width();
  it.toFirst();
  for ( ; it.current(); ++it)
    it.current()->setFixedWidth(w);
}


void ZoneClockPanel::updateTimer()
{
  TQPtrListIterator<ZoneClock> it(_clocks);
  for ( ; it.current(); ++it)
    it.current()->updateTime();
}

void ZoneClockPanel::addClock(const TQString &zone)
{
  createDialog();

  _dlg->ClockCaption->setText(i18n(zone.utf8()).section('/', -1));
  for (int i=0; i<_dlg->ClockZone->count(); ++i)
    if (_dlg->ClockZone->text(i) == i18n(zone.utf8()))
      {
        _dlg->ClockZone->setCurrentItem(i);
        break;
      }

  if (_dlg->exec() == TQDialog::Accepted)
    {
      CityList cities;
      TQStringList timezones = cities.timezones();
      TQString newzone = timezones[_dlg->ClockZone->currentItem()];
      addClock(newzone, _dlg->ClockCaption->text());
      update();
    }
}


void ZoneClockPanel::save(KConfig *config)
{
  config->writeEntry("Clocks", _clocks.count());

  TQPtrListIterator<ZoneClock> it(_clocks);
  int cnt=0;
  for ( ; it.current(); ++it)
    {
      TQString n = it.current()->name();
      n = n.left(n.length()-1);
      config->writeEntry(TQString("Clock_%1_Name").tqarg(cnt), n);
      config->writeEntry(TQString("Clock_%1_Zone").tqarg(cnt), it.current()->zone());
      cnt++;
    }
}


void ZoneClockPanel::load(KConfig *config)
{
  _clocks.clear();

  int num = config->readNumEntry("Clocks", 0);

  for (int i=0; i<num; ++i)
    {
      addClock(config->readEntry(TQString("Clock_%1_Zone").tqarg(i)), config->readEntry(TQString("Clock_%1_Name").tqarg(i)));
    }
}

