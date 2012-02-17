//-----------------------------------------------------------------------------
//
// ksprite - TQCanvas based screensaver
//
// Copyright (c)  Martin R. Jones 1996
//

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <tqtimer.h>
#include <tqpushbutton.h>
#include <tqptrlist.h>
#include <tqstrlist.h>
#include <tqcanvas.h>
#include <kscreensaver.h>
#include "spriteanim.h"

//-----------------------------------------------------------------------------
class KSpriteSaver : public KScreenSaver
{
	Q_OBJECT
  
public:
    KSpriteSaver( WId id );
    virtual ~KSpriteSaver();

    void setSpeed(int speed);

private:
    void readSettings();
    void initialise();
    void blank();

protected slots:
    void slotTimeout();

protected:
    virtual void timerEvent(TQTimerEvent *);

protected:
    TQCanvas	*mCanvas;
    TQCanvasView	*mView;
    TQTimer	mTimer;
    int                 mSpeed;
    TQPtrList<SpriteGroup>  mGroups;
    TQMemArray<int>    mTimerIds;
};

class KSpriteSetup : public TQDialog
{
    Q_OBJECT
  
public:
    KSpriteSetup( TQWidget *parent = NULL, const char *name = NULL );
    ~KSpriteSetup();

protected:
    void readSettings();

private slots:
    void slotSpeed(int s);
    void slotOkPressed();
    void slotAbout();

private:
    int     speed;
    TQWidget *preview;
    KSpriteSaver *saver;
};


#endif

