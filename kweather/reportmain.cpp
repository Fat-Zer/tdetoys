#include "reportview.h"

#include <stdlib.h>
#include <tdeapplication.h>
#include <tdecmdlineargs.h>
#include <kdebug.h>
#include <tdeaboutdata.h>
#include <tdeglobal.h>
#include <tdelocale.h>
#include <dcopclient.h>

static TDECmdLineOptions options[] =
{
    { "+location", I18N_NOOP(  "METAR location code for the report" ), 0 },
    TDECmdLineLastOption
};

extern "C" KDE_EXPORT int kdemain(int argc, char *argv[])
{
    TDEAboutData aboutData("reportview", I18N_NOOP("Weather Report"),
        "0.8", I18N_NOOP("Weather Report for KWeatherService"),
        TDEAboutData::License_GPL, "(C) 2002-2003, Ian Reinhart Geiser");
    aboutData.addAuthor("Ian Reinhart Geiser", I18N_NOOP("Developer"),
        "geiseri@kde.org");
    aboutData.addAuthor("Nadeem Hasan", I18N_NOOP("Developer"),
        "nhasan@kde.org");

    TDEGlobal::locale()->setMainCatalogue( "kweather" );

    TDECmdLineArgs::init( argc, argv, &aboutData );
    TDECmdLineArgs::addCmdLineOptions( options );
    TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();

    if ( args->count() != 1 )
    {
        args->usage();
        return -1;
    }

    TDEApplication app;

    DCOPClient *client = app.dcopClient();
    client->attach();
    TQString error;
    if (!client->isApplicationRegistered("KWeatherService"))
    {
        if (TDEApplication::startServiceByDesktopName("kweatherservice",
            TQStringList(), &error))
        {
            kdDebug() << "Starting kweatherservice failed: " << error << endl;
            return -2;
        }
    }

    TQString reportLocation = args->arg( 0 );
    reportView *report = new reportView(reportLocation);
    args->clear();
    report->exec();

    delete report;

    return 0;
}
