/***************************************************************************
                          sun.h  -  Sun Rise and Set Calculations
                             -------------------
    begin                : Friday July 11 2003
    copyright            : (C) 2003 by John Ratke
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

#ifndef SUN_H
#define SUN_H

#include <tqstring.h>
#include <tqdatetime.h>
#include <krfcdate.h>

class Sun
{
	public:
		/* Constructs a Sun instance for calculating sun rise and set
		 * times.  Users should construct it once and then call one or 
		 * more of the other four public functions.
		 *
		 * Inputs are the location on earth to calculate the rise/set time 
		 * for (for example, the location of a particular weather station), 
		 * the current date, and the offset in minutes from universal time
		 * to the local time zone.
		 *
		 */
		Sun( const TQString &latitude, 
		     const TQString &longitude,
		     TQDate date = TQDate::currentDate(),
		     const int localUTCOffset = KRFCDate::localUTCOffset() );
		
		TQTime computeRiseTime();
		TQTime computeSetTime();
		TQTime computeCivilTwilightStart();
		TQTime computeCivilTwilightEnd();

	private:
		TQTime convertDoubleToLocalTime( const double time );

		const TQDate m_date;
		const double m_lat;
		const double m_lon;
		const int m_localUTCOffset;
};

#endif
