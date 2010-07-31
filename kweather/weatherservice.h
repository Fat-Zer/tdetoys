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

#ifndef _WEATHER_SERVICE
#define _WEATHER_SERVICE

#include <tqstringlist.h>
#include <dcopobject.h>
#include <tqpixmap.h>

class WeatherLib;
class StationDatabase;

class WeatherService : public TQObject, public DCOPObject
{
	Q_OBJECT
	K_DCOP

	private:
		WeatherLib  *m_weatherLib;

	private slots:
		void updated(const TQString &stationID);
		void updating(const TQString &stationID);
		void slotStationRemoved(const TQString &stationID);

	public:
		WeatherService(TQObject *parent, const char *name);
		~WeatherService();

	k_dcop_signals:
		void fileUpdating(TQString);
		void fileUpdate(TQString);
		void stationRemoved(TQString);

	k_dcop:
		TQString temperature(const TQString &stationID);
		TQString dewPoint(const TQString &stationID);
		TQString relativeHumidity(const TQString &stationID);
		TQString heatIndex(const TQString &stationID);
		TQString windChill(const TQString &stationID);
		TQString wind(const TQString &stationID);
		TQString pressure(const TQString &stationID);
		TQPixmap currentIcon(const TQString &stationID);
		TQPixmap icon(const TQString &stationID);
		TQString currentIconString(const TQString &stationID);
		TQString iconFileName(const TQString &stationID);
		TQString date(const TQString &stationID);
		TQString visibility(const TQString &stationID);
		TQStringList cover(const TQString &stationID);
		TQStringList weather(const TQString &stationID);
		bool stationNeedsMaintenance(const TQString &stationID);

		TQString stationName(const TQString &stationID);
		TQString stationCountry(const TQString &stationID);
		TQString longitude(const TQString &stationID);
		TQString latitude(const TQString &stationID);

		TQStringList findStations(float lon, float lat);

		TQString sunRiseTime(const TQString &stationID);
		TQString sunSetTime(const TQString &stationID);
		TQString civilTwilightStart(const TQString &stationID);
		TQString civilTwilightEnd(const TQString &stationID);

		void update(const TQString &stationID);
		void updateAll();
		void forceUpdate(const TQString &stationID);
		void removeStation(const TQString &stationID);
		void addStation(const TQString &stationID);
		TQStringList listStations();
		TQString stationCode( const TQString &stationName );
		
		void exit();

  private:
		enum TimeType
		{
			RISE = 0,
			SET,
			CIVIL_START,
			CIVIL_END
		};
	  
		void saveSettings();
		TQString getTime(const TQString &stationID, TimeType timeType);
		StationDatabase *stationDB;
};
#endif
