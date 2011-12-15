/***************************************************************************
			weatherlib.cpp  -  description
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

#include "config.h"
#include <tqfile.h>
#include <tqdatetime.h>
#include <textstream.h>
#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>
#include <math.h>
#include <krfcdate.h>
#include <kio/job.h>
#include <kurl.h>
#include <knotifyclient.h>
#include <ktempfile.h>
#include <unistd.h>
#include <kapplication.h>
#include <kpassivepopup.h>

#include "metar_parser.h"
#include "stationdatabase.h"
#include "sun.h"
#include "weatherlib.h"

#include "weatherlib.moc"


class WeatherLib::Data
{
	public:
		Data();
		~Data(){ if ( target ) delete target; }

		void clear();

		/** The current weather state outside */
		struct WeatherInfo wi;
		TQDateTime age;
		KTempFile *target;
		bool downloading;
		bool updated;
		KIO::Job *job;
};

WeatherLib::Data::Data()
		: target( 0 ), job( 0 )
{
	clear();
}

void WeatherLib::Data::clear()
{
	age = TQDateTime::currentDateTime();
	downloading = false;
	updated = false;
	job = 0;

	if ( target )
	{
		delete target;
		target = 0;
	}
}

WeatherLib::WeatherLib(StationDatabase *stationDB, TQObject *parent, const char *name)
	: TQObject (parent, name)
{
	KGlobal::locale()->insertCatalogue("kweather");
	
	m_StationDb    = stationDB;
	
	data.setAutoDelete( true );
}

WeatherLib::~WeatherLib()
{
}

void WeatherLib::slotCopyDone(KIO::Job* job)
{
	kdDebug(12006) << "Copy done..." << endl;
	if( job->error())
	{
		kdDebug(12006) << "Error code: " << job->error() << endl;
		//job->showErrorDialog(0L);
		if ((job->error() == KIO::ERR_COULD_NOT_CONNECT || job->error() == KIO::ERR_UNKNOWN_HOST)
		    && !hostDown)
		{
			hostDown= true;
      // no need to show a passive popup here, as below all stations will show "dunno" icon
		}
	}
	// Find the job
	TQDictIterator<Data> it( data );
	Data *d = 0L;
	for( ; it.current(); ++it )
	{
		kdDebug(12006) << "Searching for job..." << endl;
		if(it.current()->job == job)
		{
			d = it.current();
			d->downloading = false;
			if( !job->error() )
			{
				kdDebug( 12006) << "Reading: " << d->target->name() << endl;
				TQFile file( d->target->name() );
				file.open( IO_ReadOnly );
				TQTextStream *t = new TQTextStream( &file );
				//TQTextStream *t = d->target->textStream();
				if( t )
				{
					TQString s = TQString();
					while ( !t->eof() )
					{
						s += " " + t->readLine();
					}

					if ( !s.isEmpty() )
					{
						kdDebug( 12006 ) << "Parse: " << s << endl;
						MetarParser parser(m_StationDb, KGlobal::locale()->measureSystem());
						d->wi = parser.processData(d->wi.reportLocation, s);
						d->age = TQDateTime::currentDateTime().addSecs(1800);
						emit fileUpdate(d->wi.reportLocation);
						d->updated = true;
					}
					else
					{
						// File error
						kdDebug( 12006 ) << "File empty error..." << endl;
						KPassivePopup::message( i18n("KWeather Error!"),
						i18n("The temp file %1 was empty.").arg(d->target->name()),  0L,"error" );
						d->updated = false;
					}
				}
				else
				{
					// File error
					kdDebug( 12006 ) << "File read error..." << endl;
					KPassivePopup::message( i18n("KWeather Error!"),
				i18n("Could not read the temp file %1.").arg(d->target->name()),  0L,"error" );
					d->updated = false;
				}
				delete d->target;
				d->target = 0L;
				d->job = 0L;

			}
			else if( job->error()  == KIO::ERR_DOES_NOT_EXIST)
			{
				data.remove(d->wi.reportLocation);
				kdDebug( 12006 ) << "Bad station data so i am going to remove it" << endl;
				KPassivePopup::message( i18n("KWeather Error!"),
				i18n("The requested station does not exist."),  0L,"error" );
			}
			else if(job->error() == KIO::ERR_COULD_NOT_CONNECT ||
				job->error() == KIO::ERR_UNKNOWN_HOST)
			{
				kdDebug( 12006 ) << "Offline now..." << endl;
				d->clear();
				d->wi.theWeather = "dunno";
				d->wi.qsCurrentList.append(i18n("The network is currently offline..."));
				d->wi.qsCurrentList.append(i18n("Please update later."));
				emit fileUpdate(d->wi.reportLocation);
			}
			else
			{
				kdDebug( 12006 ) << "Duh?..." << endl;
			}

		}
	}
}

void WeatherLib::getData(Data *d, bool force /* try even if host was down last time*/)
{
	if(!d->downloading && (force || !hostDown) )
	{
		d->downloading = true;
		d->updated = false;
		TQString u = "http://weather.noaa.gov/pub/data/observations/metar/stations/";
		u += d->wi.reportLocation.upper().stripWhiteSpace();
		u += ".TXT";

		d->target = new KTempFile(TQString(), "-weather");
		d->target->setAutoDelete(true);
		d->target->file();

		KURL url(u);
		KURL local(d->target->name());

		d->job = KIO::file_copy( url, local, -1, true, false, false);
		d->job->addMetaData("cache", "reload"); // Make sure to get fresh info
		connect( d->job, TQT_SIGNAL( result( KIO::Job *)),
			TQT_SLOT(slotCopyDone(KIO::Job *)));
		kdDebug( 12006 ) << "Copying " << url.prettyURL() << " to "
			<< local.prettyURL() << endl;
		emit fileUpdating(d->wi.reportLocation);
	}
}

WeatherLib::Data* WeatherLib::findData(const TQString &stationID)
{
	Data *d = data[stationID];
	if (!d)
	{
		d = new Data();
		d->wi.reportLocation = stationID;
		d->wi.theWeather = "dunno";
		d->wi.qsCurrentList.append( i18n( "Retrieving weather data..." ) );
		data.insert(stationID, d);
		getData(d);
	}

	return d;
}

TQString WeatherLib::temperature(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsTemperature;
}

TQString WeatherLib::pressure(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsPressure;
}

TQString WeatherLib::wind(const TQString &stationID){
	Data *d = findData(stationID);
	return (d->wi.qsWindSpeed + " " + d->wi.qsWindDirection);
}

/**  */
TQString WeatherLib::dewPoint(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsDewPoint;
}

TQString WeatherLib::relHumidity(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsRelHumidity;
}

TQString WeatherLib::heatIndex(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsHeatIndex;
}

TQString WeatherLib::windChill(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsWindChill;
}

TQString WeatherLib::iconName(const TQString &stationID){

	TQString result("dunno");
		
	// isEmpty is true for null or 0 length strings
	if ( !stationID.isEmpty() )
	{
		Data *d = findData(stationID);
		result = d->wi.theWeather;
	}
	
	return result;
}

TQString WeatherLib::date(const TQString &stationID){
	Data *d = findData(stationID);

  if ( ! d->wi.qsDate.isValid() )
    return "";
  else
  {
    TQDateTime gmtDateTime(d->wi.qsDate, d->wi.qsTime);
    TQDateTime localDateTime = gmtDateTime.addSecs(KRFCDate::localUTCOffset() * 60);
    return KGlobal::locale()->formatDateTime(localDateTime, false, false);
  }
}

/** Returns the current cover */
TQStringList WeatherLib::cover(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsCoverList;
}

/** return the visibility */
TQString WeatherLib::visibility(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsVisibility;
}

/** return the weather text */
TQStringList WeatherLib::weather(const TQString &stationID){
	Data *d = findData(stationID);
	return d->wi.qsCurrentList;
}

bool WeatherLib::stationNeedsMaintenance(const TQString &stationID)
{
	Data *d = findData(stationID);
	return d->wi.stationNeedsMaintenance;
}

void WeatherLib::update(const TQString &stationID)
{
	// Only grab new data if its more than 50 minutes old
	Data *d = findData(stationID);

	TQDateTime timeout = TQDateTime::currentDateTime();

	kdDebug (12006) << "Current Time: " << KGlobal::locale()->formatDateTime(timeout, false, false) <<
			" Update at: " << KGlobal::locale()->formatDateTime(d->age, false, false) << endl;
	if( timeout >= d->age || !d->updated)
		getData(d, true /* try even if host was down last time */);
	else
		emit fileUpdate(d->wi.reportLocation);
}

TQStringList WeatherLib::stations()
{
	TQStringList l;
	TQDictIterator<Data> it( data );
	for( ; it.current(); ++it )
		l += it.currentKey();
	return l;
}

void WeatherLib::forceUpdate(const TQString &stationID)
{
	hostDown = false; // we want to show error message if host is still down
	Data *d = findData(stationID);
	getData( d );
}

void WeatherLib::remove(const TQString &stationID)
{
	data.remove(stationID);
	emit stationRemoved(stationID);
}
