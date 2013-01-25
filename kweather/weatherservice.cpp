/* This file is part of the KDE project
   Copyright (C) 2001-2004 Ian Geiser <geiseri@kde.org>
   Copyright (C) 2002-2004 Nadeem Hasan <nhasan@kde.org>
   Copyright (C) 2003-2004 John Ratke

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <kdebug.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kglobal.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include "weatherlib.h"
#include "weatherservice.h"
#include "stationdatabase.h"
#include "sun.h"

WeatherService::WeatherService(TQObject *parent, const char *name) : TQObject (parent, name),  DCOPObject("WeatherService")
{
	kdDebug(12006) << "Starting new service... " << endl;

	stationDB = new StationDatabase();

	m_weatherLib = new WeatherLib(stationDB, this, "WeatherLib");
	connect(m_weatherLib, TQT_SIGNAL(fileUpdating( const TQString&)),
			TQT_SLOT(updating( const TQString&)));
	connect(m_weatherLib, TQT_SIGNAL(fileUpdate( const TQString&)),
			TQT_SLOT(updated( const TQString&)));
	connect(m_weatherLib, TQT_SIGNAL(stationRemoved(const TQString&)),
			TQT_SLOT(slotStationRemoved(const TQString&)));

	TDEConfig *conf = kapp->config();

	conf->setGroup("WEATHERSTATIONS");
	TQStringList stations =conf->readListEntry("stations");
        TQStringList::Iterator it = stations.begin();
	for ( ; it != stations.end(); ++it )
		m_weatherLib->update(*it);
}

WeatherService::~WeatherService()
{
	kdDebug(12006) << "Going away... " << endl;
	// Don't need to call saveSettings() because WeatherService::exit
        // already does that. Plus it wouldn't work here anyway because
        // we can't get the config after calling kapp->quit();
	delete stationDB;
}

void WeatherService::updated(const TQString &stationID)
{
	kdDebug(12006) << "Sending update for " << stationID << endl;
	emit fileUpdate( stationID );
}

void WeatherService::updateAll()
{
	kdDebug(12006) << "Sending for all" << endl;
	TQStringList stations =  m_weatherLib->stations();
	TQStringList::ConstIterator end(stations.end());
	for  ( TQStringList::ConstIterator it = stations.begin(); it != end; ++it ) {
		update(*it);
    	}

}

void WeatherService::updating(const TQString &stationID)
{
	kdDebug(12006) << "Sending updating for " << stationID << endl;
	emit fileUpdating( stationID );
}

void WeatherService::slotStationRemoved(const TQString &stationID)
{
  kdDebug(12006) << "Sending stationRemoved for " << stationID << endl;
  emit stationRemoved( stationID );
}

TQString WeatherService::temperature(const TQString &stationID)
{
	kdDebug (12006) << "Returning " << stationID << endl;
	return m_weatherLib->temperature(stationID);
}

TQString WeatherService::dewPoint(const TQString &stationID)
{
	return m_weatherLib->dewPoint(stationID);
}

TQString WeatherService::relativeHumidity(const TQString &stationID)
{
	return m_weatherLib->relHumidity(stationID);
}

TQString WeatherService::heatIndex(const TQString &stationID)
{
	return m_weatherLib->heatIndex(stationID);
}

TQString WeatherService::windChill(const TQString &stationID)
{
	return m_weatherLib->windChill(stationID);
}

TQString WeatherService::wind(const TQString &stationID)
{
	return m_weatherLib->wind(stationID);
}

TQString WeatherService::pressure(const TQString &stationID)
{
	return m_weatherLib->pressure(stationID);
}

TQPixmap WeatherService::currentIcon(const TQString &stationID)
{
    return icon( stationID );
}

TQPixmap WeatherService::icon(const TQString &stationID)
{
	kdDebug(12006) << "Get the current weather icon.." << endl;
	TQString icon  = iconFileName(stationID);
	TQPixmap theIcon = TQPixmap(icon);
	return theIcon;
}

TQString WeatherService::currentIconString(const TQString &stationID)
{
	return m_weatherLib->iconName(stationID);
}

TQString WeatherService::iconFileName(const TQString &stationID)
{
	TQString icon  = m_weatherLib->iconName(stationID);
	icon = locate( "data", "kweather/" + icon + ".png" );
	return icon;
}

TQString WeatherService::date(const TQString &stationID)
{
	return m_weatherLib->date(stationID);
}

TQString WeatherService::visibility(const TQString &stationID)
{
	return m_weatherLib->visibility(stationID);
}

TQStringList WeatherService::cover(const TQString &stationID)
{
	return m_weatherLib->cover(stationID);
}

TQStringList WeatherService::weather(const TQString &stationID)
{
	return m_weatherLib->weather(stationID);
}

bool WeatherService::stationNeedsMaintenance(const TQString &stationID)
{
	return m_weatherLib->stationNeedsMaintenance(stationID);
}

void WeatherService::update(const TQString &stationID)
{
	m_weatherLib->update(stationID);
}

void WeatherService::forceUpdate(const TQString &stationID)
{
	m_weatherLib->forceUpdate(stationID);
}

void WeatherService::removeStation(const TQString &stationID)
{
	m_weatherLib->remove(stationID);
	saveSettings();
}

void WeatherService::addStation(const TQString &stationID)
{
	m_weatherLib->update(stationID);
	saveSettings();
}

void WeatherService::exit()
{
	saveSettings();
	kapp->quit();
}

TQStringList WeatherService::listStations()
{
	return m_weatherLib->stations();
}

void WeatherService::saveSettings()
{
	TDEConfig *conf = kapp->config();
	conf->setGroup("WEATHERSTATIONS");
	conf->writeEntry( "stations", m_weatherLib->stations());
	conf->sync();
}

TQString WeatherService::stationName(const TQString &stationID)
{
	if ( stationDB )
	{
		TQString upperStationID = stationID.upper();
		return stationDB->stationNameFromID(upperStationID);
	}
	else
		return stationID;
}
TQString WeatherService::stationCode( const TQString &stationName )
{
	if ( stationDB )
	{
		return stationDB->stationIDfromName(stationName);
	}
	else
		return stationName;
}

TQString WeatherService::stationCountry(const TQString &stationID)
{

	if ( stationDB )
	{
		TQString upperStationID = stationID.upper();
		return stationDB->stationCountryFromID(upperStationID);
	}
	else
		return stationID;
}
TQString WeatherService::longitude(const TQString &stationID)
{
	if ( stationDB )
	{
		TQString upperStationID = stationID.upper();
		return stationDB->stationLongitudeFromID(upperStationID);
	}
	else
		return "None";
}
TQString WeatherService::latitude(const TQString &stationID)
{
	if ( stationDB )
	{
		TQString upperStationID = stationID.upper();
		return stationDB->stationLatitudeFromID(upperStationID);
	}
	else
		return "None";
}

TQStringList WeatherService::findStations(float /*lon*/, float /*lat*/)
{
	TQStringList stationList;
	stationList << "KMKE" << "KPNE" << "KTPW";
	return stationList;
}

TQString WeatherService::getTime(const TQString &stationID, TimeType timeType)
{
	TQString upperStationID = stationID.upper();

	TQString latitude  = stationDB->stationLatitudeFromID(upperStationID);
	TQString longitude = stationDB->stationLongitudeFromID(upperStationID);

	if ( latitude.compare(  i18n("Unknown Station" ) ) == 0  ||
	     longitude.compare( i18n("Unknown Station" ) ) == 0 )
	{
		return i18n( "Unknown Station" );
	}
	else
	{
		Sun theSun( latitude, longitude );
		
		TQTime time;
		switch ( timeType )
		{
			case RISE:
				time = theSun.computeRiseTime();
				break;
			case SET:
				time = theSun.computeSetTime();
				break;
			case CIVIL_START:
				time = theSun.computeCivilTwilightStart();
				break;
			case CIVIL_END:
				time = theSun.computeCivilTwilightEnd();
				break;
		}

		kdDebug(12006) << "station, lat, lon, time: " << upperStationID << " " <<
			latitude << " " << longitude << " " << time << endl;

		return TDEGlobal::locale()->formatTime(time);
	}
}

TQString WeatherService::sunRiseTime(const TQString &stationID)
{
	return getTime(stationID, RISE);
}

TQString WeatherService::sunSetTime(const TQString &stationID)
{
	return getTime(stationID, SET);
}

TQString WeatherService::civilTwilightStart(const TQString &stationID)
{
	return getTime(stationID, CIVIL_START);
}

TQString WeatherService::civilTwilightEnd(const TQString &stationID)
{
	return getTime(stationID, CIVIL_END);
}



#include "weatherservice.moc"
