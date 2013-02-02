/*
 *   kmoon - a moon phase indicator
 *   Copyright (C) 1998  Stephan Kulow
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

#include <tqslider.h>
#include <tqlayout.h>
#include <klocale.h>
#include <tqwhatsthis.h>
#include <tqvbox.h>
#include <kapplication.h>
#include <tqpushbutton.h>

#include "kmoondlg.h"
#include "kmoonwidget.h"

KMoonDlg::KMoonDlg(int a, bool n, bool m, TQWidget *parent, const char *name)
    : KDialogBase(parent, name, true, i18n("Change View"),
                  Ok|Cancel|Help), angle(a), north(n), mask(m)
{
	TQWidget *page = new TQWidget( this );
	setMainWidget(page);
	TQHBoxLayout *topLayout = new TQHBoxLayout( page, 0, spacingHint() );

        TQVBox *vbox = new TQVBox(page);
        TQHBox *hbox1 = new TQHBox(vbox);
        hbox1->setSpacing(15);

	TQLabel *label = new TQLabel( i18n("View angle:"), hbox1, "caption" );
	TQString text = i18n("You can use this to rotate the moon to the correct\n"
                            "angle for your location.\n"
			    "\n"
                            "This angle is (almost) impossible to\n"
                            "calculate from any system-given data,\n"
                            "therefore you can configure how you\n"
                            "want KMoon to display your moon here.\n"
                            "The default value is 0, but it is very\n"
                            "unlikely that you would see the moon\n"
                            "at this angle.");
	TQWhatsThis::add(label, text);

	slider = new TQSlider( -90, 90, 2, angle, Qt::Horizontal, hbox1, "slider" );
	slider->setTickmarks(TQSlider::Above);
	slider->setTickInterval(45);
	slider->setEnabled(TQPixmap::defaultDepth() > 8);
	label->setEnabled(TQPixmap::defaultDepth() > 8);
	TQWhatsThis::add(slider, text);
	connect(slider, TQT_SIGNAL(valueChanged(int)), TQT_SLOT(angleChanged(int)));

	TQHBox *hbox2 = new TQHBox(vbox);
	hbox2->setSpacing(spacingHint());

        hemitoggle = new TQPushButton(hbox2);
	hemitoggle->setText(north ? i18n("Switch to Southern Hemisphere") :
			    i18n("Switch to Northern Hemisphere"));

        connect(hemitoggle, TQT_SIGNAL(clicked()), TQT_SLOT(toggleHemi()));

        mastdetoggle = new TQPushButton(hbox2);
	mastdetoggle->setText(mask ? i18n("Switch Masking Off") :
			    i18n("Switch Masking On"));

        connect(mastdetoggle, TQT_SIGNAL(clicked()), TQT_SLOT(toggleMask()));
        topLayout->addWidget(vbox);

	moon = new MoonWidget(page, "preview");
	moon->setMinimumSize(50, 50);
	moon->setMaximumSize(200,200);
	TQWhatsThis::add(moon, i18n("The moon as KMoon would display it\n"
							   "following your current setting and time."));
	topLayout->addWidget(moon);
        connect(this, TQT_SIGNAL(helpClicked()), TQT_SLOT(help()));
	// disableResize();
}

void KMoonDlg::angleChanged(int value) {
    angle = value;
    moon->setAngle(value);
}

void KMoonDlg::help() {
    kapp->invokeHelp(TQString::fromLatin1("config"));
}

void KMoonDlg::toggleHemi() {
    moon->setNorthHemi(!moon->northHemi());
    north = moon->northHemi();
    hemitoggle->setText(north ? i18n("Switch to Southern Hemisphere") :
			i18n("Switch to Northern Hemisphere"));
}

void KMoonDlg::toggleMask() {
    moon->setMask(!moon->mask());
    mask = moon->mask();
    mastdetoggle->setText(mask ? i18n("Switch Masking Off") :
			i18n("Switch Masking On"));
}

#include "kmoondlg.moc"
