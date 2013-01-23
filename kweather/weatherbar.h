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

#ifndef __KONTQSIDEBARWEATHER_H__
#define __KONTQSIDEBARWEATHER_H__

#include <konqsidebarplugin.h>
#include <kparts/part.h>
#include <kparts/factory.h>
#include <kparts/browserextension.h>
#include <tqdict.h>
#include <dcopobject.h>
#include <tqlayout.h>
#include <tqtimer.h>

class dockwidget;
class sidebarwidget;

class KonqSidebarWeather: public KonqSidebarPlugin, virtual public DCOPObject
{
    Q_OBJECT
  
    K_DCOP
public:
    KonqSidebarWeather(TDEInstance* inst, TQObject* parent, TQWidget* widgetParent,
                        TQString& desktopName_, const char* name = 0);

    ~KonqSidebarWeather();
    virtual void* provides(const TQString&);
    void emitStatusBarText(const TQString&);
    virtual TQWidget *getWidget();

    k_dcop:
    virtual void refresh(TQString);
        
protected:
    virtual void handleURL(const KURL &url);
    virtual void handlePreview(const KFileItemList& items);
    virtual void handlePreviewOnMouseOver(const KFileItem& item);

private slots:
	void update();
	
private:
    TQDict <dockwidget> m_widgets;
    sidebarwidget *m_container;
    TQTimer *timeOut;
};

#endif

