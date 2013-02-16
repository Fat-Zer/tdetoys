/***************************************************************************
			metar_parser.h  -  Metar Parser
			-------------------
begin                : Wed June 7 2004
copyright            : (C) 2004 by John Ratke
email                : jratke@comcast.net
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef METAR_PARSER_H
#define METAR_PARSER_H

#include <tdeglobal.h>
#include <tdelocale.h>
#include <krfcdate.h>

#include <tqdatetime.h>
#include <tqregexp.h>
#include <tqstringlist.h>

class StationDatabase;

struct WeatherInfo
{
	/** The current weather state outside */
	TQString theWeather;
	int clouds;
	float windMPH;
	float tempC;
	float dewC;
	bool heavy;
	TQStringList qsCoverList;
	TQStringList qsCurrentList;
	TQDate qsDate;
	TQString qsPressure;
	TQString qsTemperature;
	TQString qsDewPoint;
	TQString qsRelHumidity;
	TQTime qsTime;
	TQString qsVisibility;
	TQString qsWindSpeed;
	TQString qsWindChill;
	TQString qsHeatIndex;
	TQString qsWindDirection;
	TQString reportLocation;
	bool stationNeedsMaintenance;
};


class MetarParser
{
	public:
		MetarParser(StationDatabase *stationDB,
			    TDELocale::MeasureSystem units = TDELocale::Imperial,
			    TQDate date = TQDate::currentDate(),
			    TQTime time = TQTime::currentTime(), 
			    unsigned int localUTCOffset = KRFCDate::localUTCOffset());

		/* 
		 * Process a METAR string (the second parameter) and return a WeatherInfo struct 
		 *
		 * The first parameter is the station ICAO code, which is needed to match
		 * the code present in the METAR string in order to parse the rest of the 
		 * data correctly.  But the station code is also used to lookup the station 
		 * latitude and longitude to calculate the sunrise and sunset time to see if 
		 * the day or night icon should be used.
		 */
		struct WeatherInfo processData(const TQString &stationID, const TQString &metar);

	private:
		bool parseCover(const TQString &s);
		bool parseCurrent(const TQString &s);
		bool parseTemperature(const TQString &s);
		bool parseTemperatureTenths(const TQString &s);
		void calcTemperatureVariables();
		void removeTrailingDotZero(TQString &string);
		bool parseDate(const TQString &s);
		bool parseTime(const TQString &s);
		bool parseVisibility(TQStringList::ConstIterator it);
		bool parsePressure( const TQString &s );
		TQString parseWindDirection(const unsigned int direction);
		bool parseWindSpeed(const TQString &s);
		bool parseStationNeedsMaintenance(const TQString &s);
		void calcCurrentIcon();
		void calcWindChill();
		bool isNight(const TQString &stationID) const;
		TQString iconName( const TQString &icon ) const;

		/*
		 * Reset the internal WeatherInfo struct of the class so that
		 * processing can be run again.  (processData can be run again)
		 */
		void reset();

		StationDatabase* const m_stationDb;
		const TDELocale::MeasureSystem m_units;
		const TQDate m_date;
		const TQTime m_time;
		const unsigned int m_localUTCOffset;
		
		struct WeatherInfo weatherInfo;
		
		TQRegExp CoverRegExp;
		TQRegExp CurrentRegExp;
		TQRegExp WindRegExp;
		TQRegExp VisRegExp;
		TQRegExp VisFracRegExp;
		TQRegExp TempRegExp;
		TQRegExp TimeRegExp;
		TQRegExp DateRegExp;
		TQRegExp PressRegExp;
		TQRegExp TempTenRegExp;
};

#endif
