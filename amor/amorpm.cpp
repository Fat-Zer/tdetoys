/* amorpm.cpp
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
#include "amorpm.h"

// static
AmorPixmapManager *AmorPixmapManager::mManager = 0;

//---------------------------------------------------------------------------
//
// Constructor
//
AmorPixmapManager::AmorPixmapManager()
    : mPixmapDir(".")
{
    mPixmaps.setAutoDelete(true);
}

//---------------------------------------------------------------------------
//
// Destructor
//
AmorPixmapManager::~AmorPixmapManager()
{
}

//---------------------------------------------------------------------------
//
// Load an image into the image manager
//
// Returns:
//   pointer to pixmap if loaded successfully, 0 otherwise.
//
const TQPixmap *AmorPixmapManager::load(const TQString & img)
{
    TQPixmap *pixmap = mPixmaps.tqfind(img);

    if (!pixmap)
    {
        // pixmap has not yet been loaded.
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
        }
    }

    return pixmap;
}

//---------------------------------------------------------------------------
//
// returns a pointer to the pixmap manager.
//
AmorPixmapManager *AmorPixmapManager::manager()
{
    if (!mManager)
    {
        mManager = new AmorPixmapManager();
    }

    return mManager;
}

