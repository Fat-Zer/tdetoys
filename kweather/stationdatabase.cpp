//
//
// C++ Implementation: $MODULE$
//
// Description:
//
//
// Author: ian reinhart geiser <geiseri@yahoo.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "stationdatabase.h"

#include <tqstringlist.h>
#include <tqfile.h>
#include <kdebug.h>

class StationInfo
{
	public:
		TQString cityName;
		TQString country;
		TQString longitude;
		TQString latitude;
		StationInfo () {}
};

StationDatabase::StationDatabase(const TQString path) : mPath(path)
{
}


StationDatabase::~StationDatabase()
{
}

bool StationDatabase::loadStation( const TQString & stationID )
{
	TQFile file( mPath );
	bool found = FALSE;

	if ( !file.open( IO_ReadOnly ) )
		return false;

	TQTextStream stream( &file );
	stream.setEncoding( TQTextStream::UnicodeUTF8 );
	TQString line;
	while ( !stream.eof() )
	{
		line = stream.readLine(); // line of text excluding '\n'
		TQStringList data = TQStringList::split( ";", line, true );

		if ( data[ 0 ].stripWhiteSpace() == stationID )
		{
			StationInfo station;
			station.cityName = data[ 3 ].stripWhiteSpace();
			station.country = data[ 5 ].stripWhiteSpace();
			station.latitude = data[ 7 ].stripWhiteSpace();
			station.longitude = data[ 8 ].stripWhiteSpace();

			theDB.insert( data[ 0 ], station );
			found = TRUE;
			break;
		}
	}

	file.close();
	return found;
}

/*!
    \fn StationDatabase::stationNameFromID(const TQString& id)
 */
TQString StationDatabase::stationNameFromID( const TQString & stationID )
{
	TQString result;
	
	if ( theDB.find( stationID ) == theDB.end() )
	{
		if ( loadStation( stationID ) )
			result = theDB[ stationID ].cityName;
		else
			result = i18n( "Unknown Station" );
	}
	else
	{
		result = theDB[ stationID ].cityName;
	}

	return result;
}

/*!
    \fn StationDatabase::stationLongitudeFromID( const TQString &stationID)
 */
TQString StationDatabase::stationLongitudeFromID( const TQString & stationID )
{
	TQString result;
	
	if ( theDB.find( stationID ) == theDB.end() )
	{
		if ( loadStation( stationID ) )
			result = theDB[ stationID ].longitude;
		else
			result = i18n( "Unknown Station" );
	}
	else
	{
		result = theDB[ stationID ].longitude;
	}

	return result;
}

/*!
    \fn StationDatabase::stationLatitudeFromID(const TQString &stationID)
 */
TQString StationDatabase::stationLatitudeFromID( const TQString & stationID )
{
	TQString result;
	
	if ( theDB.find( stationID ) == theDB.end() )
	{
		if ( loadStation( stationID ) )
			result = theDB[ stationID ].latitude;
		else
			result = i18n( "Unknown Station" );
	}
	else
	{
		result = theDB[ stationID ].latitude;
	}

	return result;
}

/*!
    \fn StationDatabase::stationCountryFromID( const TQString &stationID)
 */
TQString StationDatabase::stationCountryFromID( const TQString &stationID )
{
	TQString result;
	
	if ( theDB.find( stationID ) == theDB.end() )
	{
		if ( loadStation( stationID ) )
			result = theDB[ stationID ].country;
		else
			result = i18n( "Unknown Station" );
	}
	else
	{
		result = theDB[ stationID ].country;
	}

	return result;
}

TQString StationDatabase::stationIDfromName( const TQString &name )
{
	TQMap<TQString,StationInfo>::Iterator itr = theDB.begin();
	for( ; itr != theDB.end(); ++itr)
	{
	  kdDebug() << "Checking " << itr.data().cityName << endl;
	  if( itr.data().cityName == name )
		return itr.key();
	}
	return "0000";
}
