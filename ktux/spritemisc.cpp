//-----------------------------------------------------------------------------
//
// spritemisc
//
// Copyright (c)  Martin R. Jones 1999
//

#include <kapplication.h>
#include "spritemisc.h"

TQSize SpriteRange::mFieldSize;

SpriteRange::SpriteRange(const TQString &str)
{
    set(str);
}

int SpriteRange::random() const
{
    int val = mMin;

    if (mMax != mMin)
    {
        val = (TDEApplication::random()%(mMax-mMin)) + mMin;
    }

    return val;
}

void SpriteRange::set(const TQString &str)
{
    int r = str.find("..");

    if (r > 0)
    {
        mMin = parse(str.left(r));
        mMax = parse(str.mid(r+2, 10));
    }
    else
    {
        mMin = mMax = parse(str);
    }
}

void SpriteRange::setFieldSize(const TQSize &size)
{
    mFieldSize = size;
}

int SpriteRange::parse(const TQString &str)
{
    int val = 0;

    if (str == "WIDTH")
    {
        val = mFieldSize.width();
    }
    else if (str == "HEIGHT")
    {
        val = mFieldSize.height();
    }
    else
    {
        val = str.toInt();
    }

    return val;
}

