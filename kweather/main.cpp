#include <kuniqueapplication.h>
#include <tdecmdlineargs.h>
#include <kdebug.h>
#include <tdeaboutdata.h>
#include <tdelocale.h>
#include <dcopclient.h>
#include "weatherservice.h"

int main (int argc, char *argv[])
{
	TDELocale::setMainCatalogue("tdelibs");
	TDEAboutData aboutdata("KWeatherService", I18N_NOOP("TDE"),
				"0.8", I18N_NOOP("KWeather DCOP Service"),
				TDEAboutData::License_GPL, "(C) 2002, Ian Reinhart Geiser");
	aboutdata.addAuthor("Ian Reinhart Geiser",I18N_NOOP("Developer"),"geiseri@kde.org");
	aboutdata.addAuthor("Nadeem Hasan",I18N_NOOP("Developer"),"nhasan@kde.org");

	TDECmdLineArgs::init( argc, argv, &aboutdata );
	// TDECmdLineArgs::addCmdLineOptions( options );
	KUniqueApplication::addCmdLineOptions();

	if (!KUniqueApplication::start())
	{
		kdDebug() << "dcopservice is already running!" << endl;
		return (0);
	}

	KUniqueApplication app;
	// This app is started automatically, no need for session management
	app.disableSessionManagement();
	kdDebug() << "starting dcopservice " << endl;
	new WeatherService(TQT_TQOBJECT(&app), "WeatherService");
	return app.exec();

}
