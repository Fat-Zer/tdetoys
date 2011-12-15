/****************************************************************************
** $Id$
**
** Definition of simple flow layout for custom layout example
**
** Created : 979899
**
** Copyright (C) 1997 by Trolltech AS.  All rights reserved.
**
** This file is part of an example program for TQt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FLOW_H
#define FLOW_H

#include <layout.h>
#include <tqptrlist.h>

class SimpleFlow : public TQLayout
{
public:
    SimpleFlow( TQWidget *parent, int border=0, int space=-1,
		const char *name=0 )
	: TQLayout( parent, border, space, name ),
	cached_width(0), cached_hfw(0) {}
    SimpleFlow( TQLayout* parent, int space=-1, const char *name=0 )
	: TQLayout( parent, space, name ),
	cached_width(0), cached_hfw(0) {}
    SimpleFlow( int space=-1, const char *name=0 )
	: TQLayout( space, name ),
	cached_width(0), cached_hfw(0) {}

    ~SimpleFlow();

    void addItem( TQLayoutItem *item);
    bool hasHeightForWidth() const;
    int heightForWidth( int ) const;
    TQSize sizeHint() const;
    TQSize minimumSize() const;
    TQLayoutIterator iterator();
    TQSizePolicy::ExpandData expanding() const;

#ifdef USE_QT4
	QLAYOUT_REQUIRED_METHOD_DECLARATIONS
#endif // USE_QT4

protected:
    void setGeometry( const TQRect& );

private:
    int doLayout( const TQRect&, bool testonly = FALSE );
    TQPtrList<TQLayoutItem> list;
    int cached_width;
    int cached_hfw;
};

#endif
