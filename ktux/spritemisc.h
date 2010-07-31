//-----------------------------------------------------------------------------
//
// spritemisc
//
// Copyright (c)  Martin R. Jones 1999
//

#ifndef __SPRITEMISC_H__
#define __SPRITEMISC_H__

#include <tqstring.h>
#include <tqsize.h>

class SpriteRange
{
public:
    SpriteRange(const TQString &str);
    SpriteRange(int l=0, int h=0) : mMin(l), mMax(h) {}

    int min() const { return mMin; }
    int max() const { return mMax; }

    int random() const;

    void set(int l, int h) { mMin=l; mMax=h; }
    void set(const TQString &str);

    static void setFieldSize(const TQSize &size);
    static TQSize fieldSize() { return mFieldSize; }

protected:
    int parse(const TQString &str);

protected:
    int             mMin;
    int             mMax;
    static TQSize    mFieldSize;
};

#endif

