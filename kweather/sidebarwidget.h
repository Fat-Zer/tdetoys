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

#ifndef __SIDEBARWIDGET_H__
#define __SIDEBARWIDGET_H__


#include <sidebarwidgetbase.h>
#include <tqvbox.h>

class sidebarwidget : public sidebarwidgetbase
{
Q_OBJECT
  
public:
    sidebarwidget(TQWidget* parent, const char* name = 0);
    virtual ~sidebarwidget();
    void addWidget(TQWidget *w, const TQString &s);
    TQWidget *viewport();
    TQVBox *view;

};

#endif


