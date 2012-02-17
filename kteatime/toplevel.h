/*
 *   This file is part of the KTeaTime application.
 *
 *   Copyright (C) 1998-1999  Matthias Hoelzer-Kluepfel (hoelzer@kde.org)
 *   Copyright (C) 2002-2003  Martin Willers (willers@xm-arts.de)
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

#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include <kapplication.h>
#include <tqpopupmenu.h>
#include <tqtimer.h>
#include <tqlineedit.h>
#include <tqvaluevector.h>
#include <tqlistview.h>
#include <tqpushbutton.h>
#include <tqgroupbox.h>
#include <knuminput.h>
#include <ksystemtray.h>
#include <tqpixmap.h>

class KAction;
class KDialogBase;
class TQCheckBox;
class TimeEdit;

class TopLevel : public KSystemTray
{
	Q_OBJECT
  

public:

	TopLevel();
	~TopLevel();

protected:

	void paintEvent(TQPaintEvent *);
	void mousePressEvent(TQMouseEvent *);
	void timerEvent(TQTimerEvent *);
	void resizeEvent(TQResizeEvent *);

private slots:

	void teaSelected(int index);
	void teaStartSelected(int index);
	void start();
	void stop();
	void config();
	void help();
	void anonymous();
	void setToolTip(const TQString &text, bool force=false);
	void rebuildTeaMenus();

	void listBoxItemSelected();
	void nameEditTextChanged(const TQString& newText);
	void spinBoxValueChanged(int v);
	void newButtonClicked();
	void delButtonClicked();
	void upButtonClicked();
	void downButtonClicked();
	void confButtonClicked();
	void enable_menuEntries();
	void disable_properties();
	void enable_properties();
	void enable_controls();
	void actionEnableToggled(bool on);

private:

	static const int DEFAULT_TEA_TIME;

	struct tea_struct {
		TQString name;
		int time;
	};
	TQValueVector<tea_struct> teas;      // list of tea-names and times

	bool running, ready, firstFrame, listempty;
	int seconds;                        // variable for counting down seconds
	int startSeconds;                   // steeping time for current tea
	int percentDone;                    // ok, this isn't really "per 100", but "per 360"

	unsigned current_selected;          // index of currently +selected+ tea in menu
	TQListViewItem *current_item;        // ptr to currently +selected+ tea in ListView
	TQString current_name;               // name of currently +running+ tea (if any)
	bool shooting;                      // anonymous tea currently steeping?

	bool useNotify, usePopup, useAction;
	TQString action;
	bool useTrayVis;                    // visualize progress in tray icon

	TQPixmap mugPixmap, teaNotReadyPixmap, teaAnim1Pixmap, teaAnim2Pixmap;

	KAction *startAct, *stopAct, *confAct, *anonAct;
	TQPopupMenu *menu, *steeping_menu, *start_menu;
	TQListView *listbox;
	TQLineEdit *nameEdit, *actionEdit;
	TimeEdit *timeEdit;
	TQGroupBox *editgroup;
	TQPushButton *btn_new, *btn_del, *btn_up, *btn_down, *btn_conf;

	TQString lastTip;
	KDialogBase *anondlg, *confdlg;
	TimeEdit *anon_time;
	TQCheckBox *eventEnable, *popupEnable, *actionEnable, *visEnable;
};

#endif
