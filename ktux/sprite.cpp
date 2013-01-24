//-----------------------------------------------------------------------------
//
// KTux - TQCanvas based screensaver
//
// Copyright (c)  Martin R. Jones 1999
//

#include <stdlib.h>
#include <time.h>
#include <tqlabel.h>
#include <tqmessagebox.h>
#include <tqlayout.h>
#include <tqslider.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kstdguiitem.h>
#include <kbuttonbox.h>

#include "spritepm.h"
#include "spritemisc.h"
#include "sprite.h"
#include "sprite.moc"


// libkscreensaver interface
extern "C"
{
    KDE_EXPORT const char *kss_applicationName = "ktux";
    KDE_EXPORT const char *kss_description = I18N_NOOP( "Tux Screen Saver" );
    KDE_EXPORT const char *kss_version = "1.0.0";

    KDE_EXPORT KScreenSaver *kss_create( WId id )
    {
        return new KSpriteSaver( id );
    }

    KDE_EXPORT TQDialog *kss_setup()
    {
        return new KSpriteSetup();
    }
}

//-----------------------------------------------------------------------------

KSpriteSetup::KSpriteSetup( TQWidget *parent, const char *name )
  : TQDialog( parent, name, TRUE )
{
    TDEGlobal::locale()->insertCatalogue("ktux");
    saver = 0;

    readSettings();

    setCaption(i18n("Setup KTux") );

    TQVBoxLayout *tl = new TQVBoxLayout(this, 10, 10);
    TQHBoxLayout *tl1 = new TQHBoxLayout;
    tl->addLayout(tl1);
    TQVBoxLayout *tl11 = new TQVBoxLayout(5);
    tl1->addLayout(tl11);

    TQLabel *label = new TQLabel( i18n("Speed:"), this );
    label->setMinimumSize(label->sizeHint());
    tl11->addStretch(1);
    tl11->addWidget(label);

    TQSlider *sb = new TQSlider(0, 100, 10, speed, Qt::Horizontal, this );
    tl11->addWidget(sb);
    connect( sb, TQT_SIGNAL( valueChanged( int ) ), TQT_SLOT( slotSpeed( int ) ) );

    preview = new TQWidget( this );
    preview->setFixedSize( 220, 170 );
    preview->setBackgroundColor( black );
    preview->show();    // otherwise saver does not get correct size
    saver = new KSpriteSaver( preview->winId() );
    tl1->addWidget(preview);

    KButtonBox *bbox = new KButtonBox(this);
    TQButton *button = bbox->addButton( i18n("About"));
    connect( button, TQT_SIGNAL( clicked() ), TQT_SLOT(slotAbout() ) );
    bbox->addStretch(1);

    button = bbox->addButton( KStdGuiItem::ok());
    connect( button, TQT_SIGNAL( clicked() ), TQT_SLOT( slotOkPressed() ) );

    button = bbox->addButton(KStdGuiItem::cancel());
    connect( button, TQT_SIGNAL( clicked() ), TQT_SLOT( reject() ) );
    bbox->layout();
    tl->addWidget(bbox);

    tl->freeze();
}

KSpriteSetup::~KSpriteSetup()
{
    delete saver;
}

// read settings from config file
void KSpriteSetup::readSettings()
{
    KConfig *config = TDEGlobal::config();
    config->setGroup( "Settings" );

    speed = config->readNumEntry( "Speed", 50 );
    if (speed > 100)
	speed = 100;
    else if (speed < 0)
	speed = 0;
}

void KSpriteSetup::slotSpeed(int s)
{
    speed = s;
    if (saver)
	saver->setSpeed(speed);
}

// Ok pressed - save settings and exit
void KSpriteSetup::slotOkPressed()
{
    KConfig *config = TDEGlobal::config();
    config->setGroup("Settings");
    config->writeEntry("Speed", speed);
    config->sync();
    accept();
}

void KSpriteSetup::slotAbout()
{
  TQMessageBox::message(i18n("About KTux"),
    i18n("KTux Version 1.0\n\nWritten by Martin R. Jones 1999\nmjones@kde.org"),
    i18n("OK"));
}


//-----------------------------------------------------------------------------

KSpriteSaver::KSpriteSaver( WId id ) : KScreenSaver( id )
{
    TDEGlobal::dirs()->addResourceType("sprite", KStandardDirs::kde_default("data") + "ktux/sprites/");

    initialise();
    readSettings();
    blank();

    connect(&mTimer, TQT_SIGNAL(timeout()), TQT_SLOT(slotTimeout()));
    mTimer.start(120-mSpeed, true);
}

//-----------------------------------------------------------------------------
KSpriteSaver::~KSpriteSaver()
{
    mTimer.stop();
    delete mView;
    delete mCanvas;
}

//-----------------------------------------------------------------------------
//
void KSpriteSaver::setSpeed(int speed)
{
    mSpeed = speed;
    mTimer.changeInterval(120-mSpeed);
}

//-----------------------------------------------------------------------------
// read settings from config file
//
void KSpriteSaver::readSettings()
{
    TQString str;

    KConfig *config = TDEGlobal::config();
    config->setGroup("Settings");

    mSpeed = config->readNumEntry("Speed", 50);

    TQString path = TDEGlobal::dirs()->findResourceDir( "sprite", "bg.png" );

    SpritePixmapManager::manager()->setPixmapDir(path);

    path += "spriterc";

    KSimpleConfig *mConfig = new KSimpleConfig(path, true);
    mConfig->setGroup("Config");
    TQStrList list;
    int groups = mConfig->readListEntry("Groups", list);
    mTimerIds.resize(groups);
    for (int i = 0; i < groups; i++)
    {
	kdDebug() << "Group: " << list.at(i) << endl;;
	mConfig->setGroup(list.at(i));
	SpriteGroup *obj = new SpriteGroup(mCanvas, *mConfig);
	mTimerIds[i] = startTimer(obj->refreshTime());
	mGroups.append(obj);
    }
    delete mConfig;
}

//-----------------------------------------------------------------------------
void KSpriteSaver::initialise()
{
    mCanvas = new TQCanvas();
    TQPixmap pm( locate("sprite", "bg.png") );
    mCanvas->setBackgroundPixmap( pm );
    mCanvas->resize( width(), height() );
    mView = new TQCanvasView(mCanvas);
    mView->viewport()->setBackgroundColor( black );
    mView->resize( size());
    mView->setFrameStyle( TQFrame::NoFrame );
    mView->setVScrollBarMode( TQScrollView::AlwaysOff );
    mView->setHScrollBarMode( TQScrollView::AlwaysOff );
    embed( mView );
    mView->show();
    SpriteRange::setFieldSize(mView->size());
}

//-----------------------------------------------------------------------------
void KSpriteSaver::slotTimeout()
{
    mTimer.start(120-mSpeed, true);
    SpriteGroup *grp;

    for (grp = mGroups.first(); grp; grp = mGroups.next())
    {
	grp->next();
    }

    mCanvas->advance();
}

//-----------------------------------------------------------------------------
void KSpriteSaver::timerEvent(TQTimerEvent *ev)
{
    for (unsigned i = 0; i < mTimerIds.size(); i++)
    {
	if (mTimerIds[i] == ev->timerId())
	{
	    mGroups.at(i)->refresh();
	    killTimer(ev->timerId());
	    mTimerIds[i] = startTimer(mGroups.at(i)->refreshTime());
	    break;
	}
    }
}

//-----------------------------------------------------------------------------
void KSpriteSaver::blank()
{
    setBackgroundColor( black );
    erase();
}

