/* -------------------------------------------------------------

   tealist.cpp

   (C) 2003 by Daniel Teske (teske@bigfoot.com)

 ------------------------------------------------------------- */

#include <klocale.h>
#include <tqlistview.h>
#include "tealist.h"


TQString int2time(int time)
{
	TQString str;
	if (time / 60)
		str.append(i18n("%1 min").arg(time / 60));
	if (time % 60)
          if (str.isEmpty())
            str.append(i18n("%1 s").arg(time % 60));
          else
            str.append(i18n(" %1 s").arg(time % 60));
	return str;
}


TeaListItem::TeaListItem(TQListView * parent)
    :TQListViewItem(parent)
{

}

TeaListItem::TeaListItem(TQListView * parent, TQListViewItem *after)
    :TQListViewItem(parent, after)
{

}

TeaListItem::~TeaListItem()
{
}

void TeaListItem::setTime(int t)
{
	TQListViewItem::setText(1, int2time(t));
	tim = t;
}

void TeaListItem::setName(const TQString &n)
{
	nam = n;
	TQListViewItem::setText(0, n);
}

TQString TeaListItem::name()
{
	return nam;
}

int TeaListItem::time()
{
	return tim;
}
