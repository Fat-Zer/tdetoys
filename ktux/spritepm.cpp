//---------------------------------------------------------------------------
//
// spritepm.cpp
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#include <stdlib.h>
#include <kdebug.h>
#include "spritepm.h"

// static
SpritePixmapManager *SpritePixmapManager::mManager = 0;

//---------------------------------------------------------------------------
//
// Constructor
//
SpritePixmapManager::SpritePixmapManager()
    : mPixmapDir(".")
{
    mPixmaps.setAutoDelete(true);
}

//---------------------------------------------------------------------------
//
// Destructor
//
SpritePixmapManager::~SpritePixmapManager()
{
}

//---------------------------------------------------------------------------
//
// Load an image into the image manager
//
// Returns:
//   pointer to pixmap if loaded successfully, 0 otherwise.
//
const TQPixmap *SpritePixmapManager::load(const TQString & img)
{
    TQPixmap *pixmap = mPixmaps.find(img);

    if (!pixmap)
    {
        // pixmap has not yet been loaded.
        kdDebug() << "Reading pixmap: " << img << endl;
        TQString path = mPixmapDir + TQString("/") + img;
        pixmap = new TQPixmap(path);

        if (!pixmap->isNull())
        {
            mPixmaps.insert(img,pixmap);
        }
        else
        {
            delete pixmap;
            pixmap = 0;
            kdDebug() << "read failed" << endl;;
        }
    }

    return pixmap;
}

//---------------------------------------------------------------------------
//
// returns a pointer to the pixmap manager.
//
SpritePixmapManager *SpritePixmapManager::manager()
{
    if (!mManager)
    {
        mManager = new SpritePixmapManager();
    }

    return mManager;
}

//===========================================================================
//
SpritePixmapSequence::SpritePixmapSequence(TQPtrList<TQPixmap> pm, TQPtrList<TQPoint> hs,
        TQMemArray<int> d)
    : TQCanvasPixmapArray(pm, hs), mDelays(d)
{
}

// static
SpriteSequenceManager *SpriteSequenceManager::mManager = 0;

//===========================================================================
//
// Constructor
//
SpriteSequenceManager::SpriteSequenceManager()
{
    mSprites.setAutoDelete(true);
}

//---------------------------------------------------------------------------
//
// Destructor
//
SpriteSequenceManager::~SpriteSequenceManager()
{
}

//---------------------------------------------------------------------------
//
// Load an image into the sprite manager
//
// Returns:
//   pointer to sprite if loaded successfully, 0 otherwise.
//
SpritePixmapSequence *SpriteSequenceManager::load(TDEConfigBase &config,
    const TQString & name)
{
    SpritePixmapSequence *sprite = mSprites.find(name);

    if (!sprite)
    {
        kdDebug() << "Reading sprite: " << name << endl;
        config.setGroup(name);
        sprite = read(config);
        if (sprite)
        {
            mSprites.insert(name, sprite);
        }
    }

    return sprite;
}

//---------------------------------------------------------------------------
//
SpritePixmapSequence *SpriteSequenceManager::read(TDEConfigBase &config)
{
    TQStrList strImages;
    TQStrList strDelays;
    TQPtrList<TQPixmap> pixmaps;
    TQPtrList<TQPoint> hotspots;

    int frames = config.readListEntry("Images", strImages);
    config.readListEntry("Delays", strDelays);

    TQMemArray<int> delays(frames);

    for (int i = 0; i < frames; i++)
    {
        const TQPixmap *pixmap =
                    SpritePixmapManager::manager()->load(strImages.at(i));
        if (pixmap)
        {
            pixmaps.append(pixmap);
            hotspots.append(new TQPoint(0,0));
            delays[i] = atoi(strDelays.at(i));
        }
    }

    return new SpritePixmapSequence(pixmaps, hotspots, delays);
}

//---------------------------------------------------------------------------
//
// returns a pointer to the sprite manager.
//
SpriteSequenceManager *SpriteSequenceManager::manager()
{
    if (!mManager)
    {
        mManager = new SpriteSequenceManager();
    }

    return mManager;
}

