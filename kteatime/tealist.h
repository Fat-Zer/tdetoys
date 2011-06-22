/* -------------------------------------------------------------

   tealist.h

   (C) 2003 by Daniel Teske (teske@bigfoot.com)

 ------------------------------------------------------------- */
#ifndef TEALIST_H
#define TEALIST_H

#include <tqstring.h>

class TQListView;
class TQListViewItem;


class TeaListItem : public TQListViewItem
{

public:
	TeaListItem(TQListView *tqparent);
	TeaListItem(TQListView *tqparent, TQListViewItem *after);
	~TeaListItem();

	int time();
	TQString name();
	void setTime(int v);
	void setName(const TQString &n);

private:
	int tim;
	TQString nam;
};


TQString int2time(int t);
#endif
