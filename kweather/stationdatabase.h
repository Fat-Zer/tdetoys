//
//
// C++ Interface: $MODULE$
//
// Description:
//
//
// Author: ian reinhart geiser <geiseri@yahoo.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STATIONDATABASE_H
#define STATIONDATABASE_H

#include <tqmap.h>
#include <tqstring.h>

#include <klocale.h>
#include <kstandarddirs.h>

/**
This is the main database for mapping METAR codes to Station information.

@author ian reinhart geiser
*/

class StationInfo;

class StationDatabase
{
public:
    StationDatabase(const TQString path = locate("data", "kweatherservice/stations.dat"));
    ~StationDatabase();

    TQString stationNameFromID(const TQString& id);

    TQString stationLongitudeFromID( const TQString &stationID);

    TQString stationLatitudeFromID(const TQString &stationID);

    TQString stationCountryFromID( const TQString &stationID);
    
    TQString stationIDfromName( const TQString &name );

private:
    TQMap<TQString, StationInfo> theDB;

    bool loadStation( const TQString & stationID );

    const TQString mPath;
};

#endif
