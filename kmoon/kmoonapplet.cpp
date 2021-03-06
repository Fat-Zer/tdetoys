/*
 *   kmoon - a moon phase indicator
 *   $Id$
 *   Copyright (C) 1998,2000  Stephan Kulow
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <tqbitmap.h>
#include <tqtooltip.h>
#include <tqpainter.h>
#include <tqpopupmenu.h>
#include <tqlayout.h>

#include <dcopclient.h>
#include <kdebug.h>
#include <tdeapplication.h>
#include <twin.h>
#include <tdestartupinfo.h>
#include <tdemessagebox.h>
#include <tdeaboutdata.h>
#include <tdelocale.h>
#include <kstandarddirs.h>
#include <tdecmdlineargs.h>
#include <kiconloader.h>
#include <kiconeffect.h>
#include <tdeconfig.h>
#include <kdialogbase.h>

#include "version.h"
#include "kmoondlg.h"
#include "kmoonwidget.h"
#include "kmoonapplet.h"

const char *description = I18N_NOOP("Moon Phase Indicator for TDE");

extern "C"
{
  KDE_EXPORT KPanelApplet *init(TQWidget *parent, const TQString& configFile)
  {
    TDEGlobal::locale()->insertCatalogue("kmoon");
    return new MoonPAWidget(configFile, KPanelApplet::Normal,
			    KPanelApplet::About|KPanelApplet::Preferences,
			    parent, "kmoonapplet");
  }
}

MoonPAWidget::MoonPAWidget(const TQString& configFile, Type type, int actions,
			   TQWidget *parent, const char *name)
  : KPanelApplet(configFile, type, actions, parent, name)
{
    TDEConfig *config = TDEGlobal::config();
    config->setGroup("General");

    TQVBoxLayout *vbox = new TQVBoxLayout(this, 0,0);
    moon = new MoonWidget(this);
    moon->setAngle(config->readNumEntry("Rotation", 0));
    moon->setNorthHemi(config->readBoolEntry("Northern", true));
    moon->setMask(config->readBoolEntry("Mask", false));
    vbox->addWidget(moon);
    startTimer(1000 * 60 * 20);

    popup = new TQPopupMenu();
    popup->insertItem(SmallIcon("kmoon"),
		      i18n("&About"), this,
		      TQT_SLOT(showAbout()));
    popup->insertItem(SmallIcon("configure"), i18n("&Configure..."), this,
		      TQT_SLOT(settings()));

    // missuse timerEvent for initialising
    timerEvent(0);
}

MoonPAWidget::~MoonPAWidget()
{
  delete moon;
  delete popup;
}

void MoonPAWidget::showAbout()
{
    KDialogBase *dialog = new KDialogBase(
                                i18n("About Moon Phase Indicator"),
                                KDialogBase::Yes,
                                KDialogBase::Yes, KDialogBase::Yes,
                                0, "about", true, true,
                                KStdGuiItem::ok() );

    TQPixmap ret = DesktopIcon("kmoon");
    TQString text = i18n(description) + TQString::fromLatin1("\n\n") +
		   i18n("Written by Stephan Kulow <coolo@kde.org>\n"
                            "\n"
                            "Made an applet by M G Berberich "
                            "<berberic@fmi.uni-passau.de>\n"
                            "\n"
                            "Lunar code by Chris Osburn "
                            "<chris@speakeasy.org>\n"
                            "\n"
                            "Moon graphics by Tim Beauchamp "
                            "<timb@googol.com>");

    dialog->setIcon(ret);

    KMessageBox::createKMessageBox(dialog, ret, text, TQStringList(), TQString(), 0, KMessageBox::Notify);
}

void MoonPAWidget::settings()
{
	KMoonDlg dlg(moon->angle(), moon->northHemi(), moon->mask(),
		     this, "moondlg");
	if (dlg.exec() == KMoonDlg::Accepted) {
		moon->setAngle(dlg.getAngle());
                moon->setNorthHemi(dlg.getNorthHemi());
		moon->setMask(dlg.getMask());
		TDEConfig *config = TDEGlobal::config();
		config->setGroup("General");
		config->writeEntry("Rotation", moon->angle());
                config->writeEntry("Northern", moon->northHemi());
                config->writeEntry("Mask", moon->mask());
                config->sync();
	}
	repaint();
}

void MoonPAWidget::timerEvent( TQTimerEvent * )
{
    time_t clock;
    time(&clock);
    struct tm *t = localtime(&clock);
    moon->calcStatus(mktime(t));
    moon->repaint();
}

void MoonPAWidget::mousePressEvent( TQMouseEvent *e)
{
    if (!popup)
      return;

    if (e->button() == Qt::RightButton) {
      popup->popup(mapToGlobal(e->pos()));
      popup->exec();
    }
    if (e->button() == Qt::LeftButton) {
      showAbout();
    }
}

#include "kmoonapplet.moc"
