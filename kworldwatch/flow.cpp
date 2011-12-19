/****************************************************************************
** $Id$
**
** Implementing your own tqlayout: flow example
**
** Copyright (C) 1996 by Trolltech AS.  All rights reserved.
**
** This file is part of an example program for TQt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "flow.h"

class SimpleFlowIterator :public TQGLayoutIterator
{
public:
    SimpleFlowIterator( TQPtrList<TQLayoutItem> *l ) :idx(0), list(l)  {}
    uint count() const;
    TQLayoutItem *current();
    TQLayoutItem *next();
    TQLayoutItem *takeCurrent();

private:
    int idx;
    TQPtrList<TQLayoutItem> *list;

};

uint SimpleFlowIterator::count() const
{
    return list->count();
}

TQLayoutItem *SimpleFlowIterator::current()
{
    return idx < int(count()) ? list->at(idx) : 0;
}

TQLayoutItem *SimpleFlowIterator::next()
{
    idx++; return current();
}

TQLayoutItem *SimpleFlowIterator::takeCurrent()
{
    return idx < int(count()) ? list->take( idx ) : 0;
}

SimpleFlow::~SimpleFlow()
{
    deleteAllItems();
}


int SimpleFlow::heightForWidth( int w ) const
{
    if ( cached_width != w ) {
	//Not all C++ compilers support "mutable" yet:
	SimpleFlow * mthis = (SimpleFlow*)this;
	int h = mthis->doLayout( TQRect(0,0,w,0), TRUE );
	mthis->cached_hfw = h;
	mthis->cached_width = w;
	return h;
    }
    return cached_hfw;
}

void SimpleFlow::addItem( TQLayoutItem *item)
{
    list.append( TQT_TQLAYOUTITEM(item) );
}

bool SimpleFlow::hasHeightForWidth() const
{
    return TRUE;
}

TQSize SimpleFlow::sizeHint() const
{
    return minimumSize();
}

TQSizePolicy::ExpandData SimpleFlow::expanding() const
{
#ifdef USE_QT4
    return (Qt::Orientation)TQSizePolicy::NoDirection;
#else // USE_QT4
    return TQSizePolicy::NoDirection;
#endif // USE_QT4
}

TQLayoutIterator SimpleFlow::iterator()
{
    // [FIXME]
#ifdef USE_QT4
    #warning [FIXME] ContainerAreaLayout iterators may not function correctly under Qt4
    return TQLayoutIterator(this);	    	// [FIXME]
#else // USE_QT4
    return TQLayoutIterator( new SimpleFlowIterator( &list ) );
#endif // USE_QT4
}

void SimpleFlow::setGeometry( const TQRect &r )
{
    TQLayout::setGeometry( r );
    doLayout( r );
}

int SimpleFlow::doLayout( const TQRect &r, bool testonly )
{
    int x = r.x();
    int y = r.y();
    int h = 0;		//height of this line so far.
    TQPtrListIterator<TQLayoutItem> it(list);
    TQLayoutItem *o;
    while ( (o=it.current()) != 0 ) {
	++it;
	int nextX = x + o->sizeHint().width() + spacing();
	if ( nextX - spacing() > r.right() && h > 0 ) {
	    x = r.x();
	    y = y + h + spacing();
	    nextX = x + o->sizeHint().width() + spacing();
	    h = 0;
	}
	if ( !testonly )
	    o->setGeometry( TQRect( TQPoint( x, y ), o->sizeHint() ) );
	x = nextX;
	h = TQMAX( h,  o->sizeHint().height() );
    }
    return y + h - r.y();
}

TQSize SimpleFlow::minimumSize() const
{
    TQSize s(0,0);
    TQPtrListIterator<TQLayoutItem> it(list);
    TQLayoutItem *o;
    while ( (o=it.current()) != 0 ) {
	++it;
	s = s.expandedTo( o->minimumSize() );
    }
    return s;
}

#ifdef USE_QT4
/*!
    \reimp
*/
int SimpleFlow::count() const {
	return list.count();
}

/*!
    \reimp
*/
TQLayoutItem* SimpleFlow::itemAt(int index) const {
	return index >= 0 && index < list.count() ? (const_cast<TQPtrList<TQLayoutItem>&>(list).at(index)) : 0;
}

/*!
    \reimp
*/
TQLayoutItem* SimpleFlow::takeAt(int index) {
	if (index < 0 || index >= list.count())
		return 0;
	TQLayoutItem *item = list.at(index);
	list.remove(list.at(index));
	delete item;

	invalidate();
	return item;
}
#endif // USE_QT4

