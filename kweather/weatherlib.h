/***************************************************************************
                          weatherlib.h  -  description
                             -------------------
    begin                : Wed Jul 5 2000
    copyright            : (C) 2000 by Ian Reinhart Geiser
    email                : geiseri@msoe.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WEATHERLIB_H
#define WEATHERLIB_H
#include <tqobject.h>
#include <tqstring.h>
#include <tqregexp.h>
#include <tqdict.h>

namespace KIO
{
	class Job;
}

class StationDatabase;

class WeatherLib : public TQObject
{
	Q_OBJECT
  
	
	public:
		class Data;

		WeatherLib(StationDatabase *stationDB, TQObject *parent =0L, const char *name =0L);
		virtual ~WeatherLib();

		TQString temperature(const TQString &stationID);
		TQString dewPoint(const TQString &stationID);
		TQString relHumidity(const TQString &stationID);
		TQString heatIndex(const TQString &stationID);
		TQString windChill(const TQString &stationID);
		TQString wind(const TQString &stationID);
		TQString pressure(const TQString &stationID);
		TQString iconName(const TQString &stationID);
		TQString date(const TQString &stationID);
		TQStringList weather(const TQString &stationID);
		TQString visibility(const TQString &stationID);
		TQStringList cover(const TQString &stationID);
		bool stationNeedsMaintenance(const TQString &stationID);
		
		TQStringList stations();
		bool isNight(const TQString &stationID) const;
		
		void update(const TQString &stationID);
		void forceUpdate(const TQString &stationID);
		void remove(const TQString &stationID);
		
	signals:
		void fileUpdating(const TQString &stationID);
		void fileUpdate(const TQString &stationID);
		void stationRemoved(const TQString &stationID);

	private slots:
		void slotCopyDone(KIO::Job*);

	private:
		Data* findData(const TQString &stationID);
		void clearData(Data *d);
		void getData(Data *d, bool force = false);
		void processData(const TQString &metar, Data *d);

		StationDatabase *m_StationDb;

		TQDict<Data> data;
		bool fileDownloaded;
		bool hostDown;
};

#endif
