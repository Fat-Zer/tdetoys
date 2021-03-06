/***************************************************************************
                          dockwidget.cpp  -  description
                             -------------------
    begin                : Thu Jul 6 2000
    copyright            : (C) 2000-2003 by Ian Reinhart Geiser
                         : (C) 2002-2003 Nadeem Hasan <nhasan@kde.org>
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

#include "dockwidget.h"
#include "weatherbutton.h"
#include "weatherservice_stub.h"

#include <tqtooltip.h>
#include <tqlayout.h>
#include <tqlabel.h>
#include <tqtimer.h>
#include <tqobjectlist.h>

#include <kdebug.h>
#include <tdeglobalsettings.h>
#include <tdelocale.h>

dockwidget::dockwidget(const TQString &location, TQWidget *parent,
        const char *name) : TQWidget(parent,name), m_locationCode( location ), m_orientation(Qt::Horizontal )
{
    m_font = TDEGlobalSettings::generalFont();
    setBackgroundOrigin( AncestorOrigin );
    initDock();
    connect(m_button, TQT_SIGNAL( clicked() ), TQT_SIGNAL( buttonClicked() ));

    m_weatherService = new WeatherService_stub( "KWeatherService", "WeatherService" );
}

dockwidget::~dockwidget()
{
    delete m_weatherService;
}

void dockwidget::setLocationCode(const TQString &locationCode)
{
    m_locationCode = locationCode;
    showWeather();
}

void dockwidget::setViewMode(int _mode)
{
    kdDebug(12004) << "View mode " << _mode << endl;
    m_mode = _mode;
    if (m_mode == ShowIconOnly)
    {
        m_lblTemp->hide();
        m_lblWind->hide();
        m_lblPres->hide();
    }
    else if (m_mode == ShowTempOnly)
    {
        m_lblTemp->show();
        m_lblWind->hide();
        m_lblPres->hide();
    }
    else if (m_mode == ShowAll)
    {
        m_lblTemp->show();
        m_lblWind->show();
        m_lblPres->show();
    }
}

void dockwidget::showWeather()
{
    TQString tip = "<qt>";

    TQString temp     = "?";
    TQString wind     = "?";
    TQString pressure = "?";

    if ( !m_locationCode.isEmpty() )
    {
        temp     = m_weatherService->temperature( m_locationCode );
        wind     = m_weatherService->wind( m_locationCode );
        pressure = m_weatherService->pressure( m_locationCode );

        TQString dewPoint    = m_weatherService->dewPoint( m_locationCode);
        TQString relHumidity = m_weatherService->relativeHumidity( m_locationCode );
        TQString heatIndex   = m_weatherService->heatIndex( m_locationCode );
        TQString windChill   = m_weatherService->windChill( m_locationCode );
        TQString sunRiseTime = m_weatherService->sunRiseTime( m_locationCode );
        TQString sunSetTime  = m_weatherService->sunSetTime( m_locationCode );

        tip += "<h3><center><nobr>" +
               m_weatherService->stationName( m_locationCode ) + " (" +
               m_weatherService->stationCountry( m_locationCode ) + ")</nobr></center></h3>";

        if ( m_weatherService->currentIconString( m_locationCode ) == "dunno" )  // no data
            tip += "<center><nobr>" + i18n("The network is currently offline...") + "</nobr></center>";

        tip += TQString("<br><table>"
                "<tr><th><nobr>" + i18n( "Temperature:"   ) + "</nobr></th><td><nobr>%1</nobr></td>"
                    "<th><nobr>" + i18n( "Dew Point:"     ) + "</nobr></th><td><nobr>%2</nobr></td></nobr></tr>"

                "<tr><th><nobr>" + i18n( "Air Pressure:"  ) + "</nobr></th><td><nobr>%3</nobr></td>"
                    "<th><nobr>" + i18n( "Rel. Humidity:" ) + "</nobr></th><td><nobr>%4</nobr></td></nobr></tr>"

                "<tr><th><nobr>" + i18n( "Wind Speed:"    ) + "</nobr></th><td><nobr>%5</nobr></td>")
                .arg(temp).arg(dewPoint).arg(pressure).arg(relHumidity).arg(wind);

        if ( !heatIndex.isEmpty() )
            tip += TQString("<th><nobr>" + i18n( "Heat Index:" ) + "</nobr></th><td><nobr>%1</nobr></td>").arg(heatIndex);
        else if ( !windChill.isEmpty() )
            tip += TQString("<th><nobr>" + i18n( "Wind Chill:" ) + "</nobr></th><td><nobr>%1</nobr></td>").arg(windChill);
        else
            tip += "<td>&nbsp;</td><td>&nbsp;</td>";
        tip += "</tr>";

        tip += TQString("<tr><th><nobr>" + i18n( "Sunrise:" ) + "</nobr></th><td><nobr>%1</nobr></td>" +
                           "<th><nobr>" + i18n( "Sunset:"  ) + "</nobr></th><td><nobr>%2</nobr></td>")
                 .arg(sunRiseTime).arg(sunSetTime);

        tip += "</tr></table>";

        if ( m_weatherService->stationNeedsMaintenance( m_locationCode ) )
        {
            tip += "<br>" + i18n("Station reports that it needs maintenance\n"
                                 "Please try again later");
        }
    }
    else
    {
        tip += i18n("Temperature: ") + temp + "<br>";
        tip += i18n("\nWind: ") + wind + "<br>";
        tip += i18n("\nAir pressure: ") + pressure + "<br>";
    }

    tip += "</qt>";

    // On null or empty location code, or if the station needs maintenance, this will return the dunno icon.
    TQPixmap icon = m_weatherService->icon( m_locationCode );

    TQToolTip::remove(this);
    TQToolTip::add(this, tip);

    kdDebug(12004) << "show weather: " << endl;
    kdDebug(12004) << "location: " << m_locationCode << endl;
    kdDebug(12004) << "temp,wind,pressure: " << temp << " " << wind << " " << pressure << endl;
    kdDebug(12004) << "tip: " << tip << endl;

    m_lblTemp->setText(temp);
    m_lblWind->setText(wind);
    m_lblPres->setText(pressure);

    m_button->setPixmap( icon );
}

void dockwidget::initDock()
{
    kdDebug(12004) << "Init dockwidget " << endl;

    m_button= new WeatherButton(this,"m_button");

    m_lblTemp= new TQLabel(this,"lblTemp");
    m_lblWind= new TQLabel(this,"lblWind");
    m_lblPres= new TQLabel(this,"lblPres");

    m_lblTemp->setBackgroundOrigin(AncestorOrigin);
    m_lblWind->setBackgroundOrigin(AncestorOrigin);
    m_lblPres->setBackgroundOrigin(AncestorOrigin);

    m_lblTemp->setMargin(0);
    m_lblWind->setMargin(0);
    m_lblPres->setMargin(0);

    TQBoxLayout *mainLayout = new TQBoxLayout(this, TQBoxLayout::TopToBottom);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_button, 0, TQt::AlignCenter);

    TQBoxLayout *layout = new TQBoxLayout(mainLayout, TQBoxLayout::TopToBottom);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_lblTemp);
    layout->addWidget(m_lblWind);
    layout->addWidget(m_lblPres);

    mainLayout->addSpacing(8);

    updateFont();

    TQTimer::singleShot( 0, this, TQT_SLOT( showWeather() ) );
}

/** resize the view **/
void dockwidget::resizeView( const TQSize &size )
{
    kdDebug(12004) << "Changing to size " << size << endl;
    resize(size);

    if ( m_orientation ==Qt::Horizontal ) // Kicker in horizontal mode
    {
        int h = size.height();

        if ( m_mode == ShowAll )
        {
            if ( h <= 128 )  // left to right layout
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::LeftToRight);
                m_lblTemp->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
                m_lblWind->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
                m_lblPres->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
            }
            else  // top to bottom
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::TopToBottom);
                TQFontMetrics fm(m_font);
                h = 128 - (3 * fm.height());  // 3 lines of text below the button
                m_lblTemp->setAlignment(TQt::AlignCenter);
                m_lblWind->setAlignment(TQt::AlignCenter);
                m_lblPres->setAlignment(TQt::AlignCenter);
            }
            m_button->setFixedSize(h, h);
        }
        else if ( m_mode == ShowTempOnly )
        {
            if ( h <= 32 )  // left to right
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::LeftToRight);
                m_lblTemp->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
            }
            else  // top to bottom
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::TopToBottom);
                TQFontMetrics fm(m_font);
                h = TQMIN(128, h) - fm.height();
                m_lblTemp->setAlignment(TQt::AlignCenter);
            }
            m_button->setFixedSize(h, h);
        }
        else
        {
            h = TQMIN(h, 128);
            m_button->setFixedSize(h, h);
        }
    }
    else // Kicker in vertical mode
    {
        int w = size.width();
        int h = size.height();

        if ( m_mode == ShowAll )
        {
            if ( w <= 128 )  // top to bottom
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::TopToBottom);
                m_lblTemp->setAlignment(TQt::AlignCenter);
                m_lblWind->setAlignment(TQt::AlignCenter);
                m_lblPres->setAlignment(TQt::AlignCenter);

                TQFontMetrics fm(m_font);
                h = h - (3 * fm.height());  // 3 lines of text below the button
                h = TQMIN(w, h);
            }
            else  // left to right layout
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::LeftToRight);
                m_lblTemp->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
                m_lblWind->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
                m_lblPres->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);
            }
            m_button->setFixedSize(h, h);
        }
        else if ( m_mode == ShowTempOnly )
        {
            if ( w <= 128 )  // top to bottom
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::TopToBottom);
                m_lblTemp->setAlignment(TQt::AlignCenter);

                h = w;
            }
            else  // left to right layout
            {
                static_cast<TQBoxLayout*>(layout())->setDirection(TQBoxLayout::LeftToRight);
                m_lblTemp->setAlignment(TQt::AlignAuto | TQt::AlignVCenter);

                h = static_cast<int>(w * 0.33);
            }
            m_button->setFixedSize(h, h);
        }
        else
        {
            w = TQMIN(w, 128);
            m_button->setFixedSize(w, w);
        }
    }
}

int dockwidget::widthForHeight(int h)
{
    int w;
    TQFontInfo fi(TDEGlobalSettings::generalFont());

    if ( m_mode == ShowAll )
    {
        if ( h <= 128 )  // left to right layout
        {
            int pixelSize = h/3 - 3;
            pixelSize = TQMIN(pixelSize, fi.pixelSize());  // don't make it too large
            m_font.setPixelSize(pixelSize);
            TQFontMetrics fm(m_font);
            w = h + TQMAX(fm.width(m_lblWind->text()), fm.width(m_lblPres->text())) + 1;
        }
        else  // top to bottom
        {
            if ( fi.pixelSize() * 3 <= (h/2) )  // half icon, half text
            {
                m_font = TDEGlobalSettings::generalFont();
            }
            else
            {
                m_font.setPixelSize(h/2/3);
            }
            TQFontMetrics fm(m_font);
            // size of icon
            h = 128 - (3 * fm.height());  // 3 lines of text below the button
            w = TQMAX(fm.width(m_lblWind->text()), fm.width(m_lblPres->text())) + 1;
            w = TQMAX(h, w);  // at least width of square icon
        }
    }
    else if ( m_mode == ShowTempOnly )
    {
        if ( h <= 32 )  // left to right layout
        {
            int pixelSize = h - 3;
            pixelSize = TQMIN(pixelSize, fi.pixelSize());  // don't make it too large
            m_font.setPixelSize(pixelSize);
            TQFontMetrics fm(m_font);
            w = h + fm.width(m_lblTemp->text()) + 1;
        }
        else  // top to bottom
        {
            if ( fi.pixelSize() <= (h/2) )  // half icon, half text
            {
                m_font = TDEGlobalSettings::generalFont();
            }
            else
            {
                m_font.setPixelSize(h/2);
            }
            TQFontMetrics fm(m_font);
            // size of icon
            h = TQMIN(128, h) - fm.height();
            w = fm.width(m_lblTemp->text()) + 1;
            w = TQMAX(h, w);  // at least width of square icon
        }
    }
    else
    {
        w = TQMIN(128, h);  // don't make it too large
    }

    updateFont();
    return w + 4;
}

int dockwidget::heightForWidth( int w )
{
    int h;

    if ( m_mode == ShowAll )
    {
        TQFontMetrics fmg(TDEGlobalSettings::generalFont());
        int maxWidth = fmg.width("888 km/h NNWW");  // a good approximation

        if ( w <= 128 )  // top to bottom
        {
            if ( maxWidth <= w )  // enough space to use global font
            {
                m_font = TDEGlobalSettings::generalFont();
            }
            else  // we have to reduce the fontsize
            {
                m_font.setPixelSize(static_cast<int>(fmg.height() * double(w) / maxWidth));
            }

            TQFontMetrics fm(m_font);
            h = w + (3 * fm.height());  // 3 lines of text below the button
        }
        else
        {
            if ( w >= (maxWidth * 1.5) )  // half of text width shall be icon
            {
                m_font = TDEGlobalSettings::generalFont();
            }
            else
            {
                m_font.setPixelSize(static_cast<int>(fmg.height() * (w*0.66) / maxWidth));
            }

            TQFontMetrics fm(m_font);
            h = 3 * fm.height();  // 3 lines of text

        }
    }
    else if ( m_mode == ShowTempOnly )
    {
        TQFontMetrics fmg(TDEGlobalSettings::generalFont());
        int maxWidth = fmg.width("888.88 CC");  // a good approximation

        if ( w <= 128 )  // top to bottom
        {
            if ( maxWidth <= w )  // enough space to use global font
            {
                m_font = TDEGlobalSettings::generalFont();
            }
            else  // we have to reduce the fontsize
            {
                m_font.setPixelSize(static_cast<int>(fmg.height() * double(w) / maxWidth));
            }

            TQFontMetrics fm(m_font);
            h = w + fm.height();  // text below the button
        }
        else
        {
            if ( w >= (maxWidth * 1.5) )  // half of text width shall be icon
            {
                m_font = TDEGlobalSettings::generalFont();
            }
            else
            {
                m_font.setPixelSize(static_cast<int>(fmg.height() * (w*0.66) / maxWidth));
            }

            TQFontMetrics fm(m_font);
            h = TQMAX(fm.height(), static_cast<int>(w * 0.33));
        }
    }
    else
    {
        h = TQMIN(128, w);  // don't make it too large
    }

    updateFont();
    return h;
}


void dockwidget::updateFont()
{
    //kdDebug(12004) << "Update font: " << m_font.pixelSize() << endl;
    m_lblTemp->setFont(m_font);
    m_lblWind->setFont(m_font);
    m_lblPres->setFont(m_font);
}

#include "dockwidget.moc"

// vim:ts=4:sw=4:et
