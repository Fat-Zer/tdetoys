//---------------------------------------------------------------------------
//
// spritepm.h
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef SPRITEPM_H
#define SPRITEPM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <tqdict.h>
#include <tqpixmap.h>
#include <tqcanvas.h>
#include <kconfigbase.h>
#include <ksimpleconfig.h>                                                      

//---------------------------------------------------------------------------
//
// SpritePixmapManager stores the frames used in animations.
//
class SpritePixmapManager
{
public:
    SpritePixmapManager();
    virtual ~SpritePixmapManager();

    void setPixmapDir(const TQString &dir)
        { mPixmapDir = dir; }
    void reset()
        { mPixmapDir = "."; mPixmaps.clear(); }
    const TQPixmap *load(const TQString & img);
    const TQPixmap *pixmap(const char *img) const
        { return mPixmaps.find(img); }

    static SpritePixmapManager *manager();

public:
    TQString        mPixmapDir;           // get pixmaps from here
    TQDict<TQPixmap> mPixmaps;             // list of pixmaps
    static SpritePixmapManager *mManager; // static pointer to instance
};

//---------------------------------------------------------------------------
//
class SpritePixmapSequence : public TQCanvasPixmapArray
{
public:
    SpritePixmapSequence(TQPtrList<TQPixmap> pm, TQPtrList<TQPoint> hs, TQMemArray<int> d);

    int delay(int i) const { return mDelays[i]; }

protected:
    TQMemArray<int> mDelays;
};

//---------------------------------------------------------------------------
//
// SpriteManager stores the animation sequences
//
class SpriteSequenceManager
{
public:
    SpriteSequenceManager();
    ~SpriteSequenceManager();
 
    SpritePixmapSequence *load(KConfigBase &config, const TQString & name);
    SpritePixmapSequence *sprite(const char *name)
        { return mSprites.find(name); }

    static SpriteSequenceManager *manager();
     
protected:
    SpritePixmapSequence *read(KConfigBase &config);

protected:
    TQDict<SpritePixmapSequence> mSprites;
    static SpriteSequenceManager *mManager;
};

#endif // SPRITEPM_H

