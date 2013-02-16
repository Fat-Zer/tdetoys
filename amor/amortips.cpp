/* amortips.cpp
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
#include <stdlib.h>
#include <tdeapplication.h>
#include "amortips.h"
#include <kstandarddirs.h>
#include <tdelocale.h>
#include <tqfile.h>
#include <tqregexp.h>
#include <kdebug.h>

//---------------------------------------------------------------------------
//
AmorTips::AmorTips()
{
    TDEGlobal::locale()->insertCatalogue("ktip"); // For ktip tip translations
}

//---------------------------------------------------------------------------
//
// Set the file containing tips.  This reads all tips into memory at the
// moment - need to make more efficient.
//
bool AmorTips::setFile(const TQString& file)
{
    bool rv = false;

    TQString path( locate("appdata", file) );
    if(path.length() && read(path))
        rv = true;

    rv |= readKTips();

    return rv;
}

//---------------------------------------------------------------------------
//
// Clear all tips from memory
//
void AmorTips::reset()
{
    mTips.clear();
}

//---------------------------------------------------------------------------
//
// Get a tip randomly from the list
//
TQString AmorTips::tip()
{
    if (mTips.count())
    {
        TQString tip = *mTips.at(kapp->random() % mTips.count());
		return i18n(tip.utf8());
    }

    return i18n("No tip");
}

//---------------------------------------------------------------------------
//
// Read the tips from ktip's file
//
bool AmorTips::readKTips()
{
    TQString fname;

    fname = locate("data", TQString("kdewizard/tips"));

    if (fname.isEmpty())
	return false;

    TQFile f(fname);
    if (f.open(IO_ReadOnly))
    {
	// Reading of tips must be exactly as in KTipDatabase::loadTips for translation
	TQString content = f.readAll();
	const TQRegExp rx("\\n+");

	int pos = -1;
	while ((pos = content.find("<html>", pos + 1, false)) != -1)
	{
	    TQString tip = content
		.mid(pos + 6, content.find("</html>", pos, false) - pos - 6)
		.replace(rx, "\n");
	    if (!tip.endsWith("\n"))
		tip += "\n";
	    if (tip.startsWith("\n"))
		tip = tip.mid(1);
	    if (tip.isEmpty())
	    {
		kdDebug() << "Empty tip found! Skipping! " << pos << endl;
		continue;
	    }
	    mTips.append(tip);
	}

	f.close();
    }

    return true;
}

//---------------------------------------------------------------------------
//
// Read all tips from the specified file.
//
bool AmorTips::read(const TQString& path)
{
    TQFile file(path);

    if (file.open(IO_ReadOnly))
    {
        while (!file.atEnd())
        {
            readTip(file);
        }

	return true;
    }

    return false;
}

//---------------------------------------------------------------------------
//
// Read a single tip.
//
bool AmorTips::readTip(TQFile &file)
{
    char buffer[1024] = "";
    TQString tip;

    while (!file.atEnd() && buffer[0] != '%')
    {
        file.readLine(buffer, 1024);
        if (buffer[0] != '%')
        {
            tip += TQString::fromUtf8(buffer);
        }
    }

    if (!tip.isEmpty())
    {
        if (tip[tip.length()-1] == '\n')
        {
            tip.truncate(tip.length()-1);
        }
        mTips.append(tip);
        return true;
    }

    return false;
}


