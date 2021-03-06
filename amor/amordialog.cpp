/* amordialog.cpp
**
** Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
**
*/

/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program in a file called COPYING; if not, write to
** the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
** MA 02110-1301, USA.
*/

/*
** Bug reports and questions can be sent to kde-devel@kde.org
*/

#include <tqcheckbox.h>
#include <tqlabel.h>
#include <tqslider.h>
#include <tqpainter.h>
#include <tdeapplication.h>
#include <ksimpleconfig.h>
#include "amordialog.h"
#include "amordialog.moc"
#include "version.h"
#include <tdelocale.h>
#include <tqvbox.h>
#include <kstandarddirs.h>

//---------------------------------------------------------------------------
//
// Constructor
//
AmorDialog::AmorDialog()
    : KDialogBase(0, "amordlg", false, i18n("Options"), Ok|Apply|Cancel, Ok )
{
    mConfig.read();
    TQVBox *mainwidget = makeVBoxMainWidget();

    TQHBox *hb = new TQHBox(mainwidget);

    // Theme list
    TQVBox *themeBox = new TQVBox(hb);
    themeBox->setSpacing(spacingHint());

    TQLabel *label = new TQLabel(i18n("Theme:"), themeBox);

    mThemeListBox = new TQListBox(themeBox);
    connect(mThemeListBox,TQT_SIGNAL(highlighted(int)),TQT_SLOT(slotHighlighted(int)));
    mThemeListBox->setMinimumSize( fontMetrics().maxWidth()*20,
				   fontMetrics().lineSpacing()*6 );

    mAboutEdit = new TQMultiLineEdit(themeBox);
    mAboutEdit->setReadOnly(true);
    mAboutEdit->setMinimumHeight( fontMetrics().lineSpacing()*4 );

    themeBox->setStretchFactor(mThemeListBox, 4);
    themeBox->setStretchFactor(mAboutEdit, 1);

    // Animation offset
    TQVBox *offsetBox = new TQVBox(hb);
    offsetBox->setSpacing(spacingHint());
    label = new TQLabel(i18n("Offset:"), offsetBox);

    TQSlider *slider = new TQSlider(-40, 40, 5, mConfig.mOffset,
                                    Qt::Vertical, offsetBox);
    connect(slider, TQT_SIGNAL(valueChanged(int)), TQT_SLOT(slotOffset(int)));

    // Always on top
    TQCheckBox *checkBox = new TQCheckBox(i18n("Always on top"), mainwidget);
    connect(checkBox, TQT_SIGNAL(toggled(bool)), TQT_SLOT(slotOnTop(bool)));
    checkBox->setChecked(mConfig.mOnTop);

    checkBox = new TQCheckBox(i18n("Show random tips"), mainwidget);
    connect(checkBox, TQT_SIGNAL(toggled(bool)), TQT_SLOT(slotRandomTips(bool)));
    checkBox->setChecked(mConfig.mTips); // always keep this one after the connect, or the TQList would not be grayed when it should

    checkBox = new TQCheckBox(i18n("Use a random character"), mainwidget);
    connect(checkBox, TQT_SIGNAL(toggled(bool)), TQT_SLOT(slotRandomTheme(bool)));
    checkBox->setChecked(mConfig.mRandomTheme);

    checkBox = new TQCheckBox(i18n("Allow application tips"), mainwidget);
    connect(checkBox, TQT_SIGNAL(toggled(bool)), TQT_SLOT(slotApplicationTips(bool)));
    checkBox->setChecked(mConfig.mAppTips);

    readThemes();
}

//---------------------------------------------------------------------------
//
// Destructor
//
AmorDialog::~AmorDialog()
{
}

//---------------------------------------------------------------------------
//
// Get list of all themes
//
void AmorDialog::readThemes()
{
    TQStringList files;

    // Non-recursive search for theme files, with the relative paths stored
    // in files so that absolute paths are not used.
    TDEGlobal::dirs()->findAllResources("appdata", "*rc", false, false, files);

    for (TQStringList::ConstIterator it = files.begin();
	 it != files.end();
	 it++)
      addTheme(*it);
}

//---------------------------------------------------------------------------
//
// Add a single theme to the list
//
void AmorDialog::addTheme(const TQString& file)
{
    KSimpleConfig config(locate("appdata", file), true);

    config.setGroup("Config");

    TQString pixmapPath = config.readPathEntry("PixmapPath");
    if (pixmapPath.isEmpty())
    {
        return;
    }

    pixmapPath += "/";

    if (pixmapPath[0] != '/')
    {
        // relative to config file. We add a / to indicate the dir
        pixmapPath = locate("appdata", pixmapPath);
    }

    TQString description = config.readEntry("Description");
    TQString about = config.readEntry("About", " ");
    TQString pixmapName = config.readEntry("Icon");

    pixmapPath += pixmapName;

    TQPixmap pixmap(pixmapPath);

    AmorListBoxItem *item = new AmorListBoxItem(description, pixmap);
    mThemeListBox->insertItem(item);
    mThemes.append(file);
    mThemeAbout.append(about);

    if (mConfig.mTheme == file)
    {
        mThemeListBox->setSelected(mThemeListBox->count()-1, true);
    }
}

//---------------------------------------------------------------------------
//
// User highlighted a theme
//
void AmorDialog::slotHighlighted(int index)
{
    mConfig.mTheme = *mThemes.at(index);
    mAboutEdit->setText(*mThemeAbout.at(index));
}

//---------------------------------------------------------------------------
//
// User changed offset
//
void AmorDialog::slotOffset(int off)
{
    mConfig.mOffset = off;
    emit offsetChanged(mConfig.mOffset);
}

//---------------------------------------------------------------------------
//
// User toggled always on top
//
void AmorDialog::slotOnTop(bool onTop)
{
    mConfig.mOnTop = onTop;
}

//---------------------------------------------------------------------------
//
// User toggled random tips
//
void AmorDialog::slotRandomTips(bool tips)
{
    mConfig.mTips = tips;
}

//---------------------------------------------------------------------------
//
// User toggled random character
//
void AmorDialog::slotRandomTheme(bool randomTheme)
{
    mThemeListBox->setEnabled(!randomTheme);
    mConfig.mRandomTheme = randomTheme;
}

//---------------------------------------------------------------------------
//
// User toggled application tips
//
void AmorDialog::slotApplicationTips(bool tips)
{
    mConfig.mAppTips = tips;
}

//---------------------------------------------------------------------------
//
// User clicked Ok
//
void AmorDialog::slotOk()
{
    mConfig.write();
    emit changed();
    accept();
}

//---------------------------------------------------------------------------
//
// User clicked Ok
//
void AmorDialog::slotApply()
{
    mConfig.write();
    emit changed();
}

//---------------------------------------------------------------------------
//
// User clicked Cancel
//
void AmorDialog::slotCancel()
{
    // restore offset
    TDEConfig *config = kapp->config();
    TDEConfigGroupSaver cs(config, "General");
    emit offsetChanged(config->readNumEntry("Offset"));
    reject();
}

//===========================================================================
//
// AmorListBoxItem implements a list box items for selection of themes
//
void AmorListBoxItem::paint( TQPainter *p )
{
    p->drawPixmap( 3, 0, mPixmap );
    TQFontMetrics fm = p->fontMetrics();
    int yPos;                       // vertical text position
    if ( mPixmap.height() < fm.height() )
        yPos = fm.ascent() + fm.leading()/2;
    else
        yPos = mPixmap.height()/2 - fm.height()/2 + fm.ascent();
    p->drawText( mPixmap.width() + 5, yPos, text() );
}

int AmorListBoxItem::height(const TQListBox *lb ) const
{
    return TQMAX( mPixmap.height(), lb->fontMetrics().lineSpacing() + 1 );
}

int AmorListBoxItem::width(const TQListBox *lb ) const
{
    return mPixmap.width() + lb->fontMetrics().width( text() ) + 6;
}

