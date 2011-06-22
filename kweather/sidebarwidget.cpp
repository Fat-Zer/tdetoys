   /*
 *  This file is part of the KDE project
 *  Copyright (C) 2002 Ian Reinhart Geiser <geiseri@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2.0 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 *  $Id$
 */
 
 #include "sidebarwidget.h"  
 #include <tqscrollview.h>
   
   sidebarwidget::sidebarwidget(TQWidget* tqparent, const char* name) :
   	sidebarwidgetbase(tqparent,name)
   {
   	view = new TQVBox(reportGrid->viewport());
	reportGrid->addChild(view);
	reportGrid->setResizePolicy(TQScrollView::AutoOneFit);
   }
   
    sidebarwidget::~sidebarwidget()
    {
    
    }
    
    void sidebarwidget::addWidget(TQWidget *w, const TQString &s)
    {
    	w->setFixedWidth(reportGrid->visibleWidth ());
    }
    
    TQWidget *sidebarwidget::viewport()
    {
    	return view;
    }

#include "sidebarwidget.moc"
    
