//---------------------------------------------------------------------------
//
// spriteanim.h
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <tqpixmap.h>
#include <tqdict.h>                                                              
#include <tqptrlist.h>
#include <tqstrlist.h>
#include <tqcanvas.h>
#include <kconfigbase.h>
#include <ksimpleconfig.h>                                                      

//---------------------------------------------------------------------------
//
// SpriteObject stores the animations that create an object
//
class SpriteObject : public QCanvasSprite
{
public:
    SpriteObject(SpritePixmapSequence *seq, TQCanvas *c);

    void setLifeSpan(int l) { mLifeSpan = l; }
    void age();
    bool dead() const { return (mLifeSpan == 0); }
    void setBounds( int x1, int y1, int x2, int y2 );
    bool outOfBounds() const;

protected:
    int                     mCycle;
    int                     mLifeSpan;
    SpritePixmapSequence    *mSeq;
    QRect		    mBound;
};

//---------------------------------------------------------------------------
//
// SpriteDef stores the definition of a sprite
//
class SpriteDef
{
public:
    SpriteDef(KConfigBase &config);

    SpriteObject *create( TQCanvas *c );

protected:
    void read(KConfigBase &config);

protected:
    SpriteRange             mDirX;
    SpriteRange             mDirY;
    SpriteRange             mStartX;
    SpriteRange             mStartY;
    SpriteRange             mEndX;
    SpriteRange             mEndY;
    int                     mLifeSpan;
    int                     mZ;
    SpritePixmapSequence    *mSeq;
};

//---------------------------------------------------------------------------
//
// SpriteGroup
//
class SpriteGroup
{
public:
    SpriteGroup(TQCanvas *c, KConfigBase &config);

    void next();
    void refresh();
    int refreshTime() const { return mRefresh.random(); }

protected:
    void read(KConfigBase &config);

protected:
    TQPtrList<SpriteDef>        mAvailable;
    TQPtrList<SpriteObject>     mActive;
    int                     mCount;
    SpriteRange             mRefresh;
    QCanvas		    *mCanvas;
};

#endif // SPRITEANIM_H

