/***************************************************************************
                          reportview.h  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#ifndef REPORTVIEW_H
#define REPORTVIEW_H

#include <kdialogbase.h>
#include <tqstringlist.h>


class KHTMLPart;
class WeatherService_stub;

/**This is the main report generated for the user.
  *@author Ian Reinhart Geiser
  */

class reportView : public KDialogBase {
    Q_OBJECT
  TQ_OBJECT

public:
    reportView(const TQString &reportLocation);
    ~reportView();

    void render();

private:
    WeatherService_stub *m_weatherService;
    KHTMLPart *m_reportView;
    TQString m_locationCode;
};

#endif
