/*
**
** Copyright (C) 1998-2001 by Matthias Hölzer-Klüpfel <hoelzer@kde.org>
**	Maintainence has ceased - send questions to kde-devel@kde.org.
**
*/

/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program in a file called COPYING; if not, write to
** the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
** MA 02110-1301, USA.
*/

/*
** Bug reports and questions can be sent to kde-devel@kde.org
*/
#include <stdlib.h>


#include <tqlabel.h>
#include <tqlayout.h>
#include <tqpainter.h>
#include <tqpixmap.h>
#include <tqwidget.h>


#include <tdeapplication.h>
#include <tdeconfig.h>
#include <tdeaboutdata.h>
#include <tdecmdlineargs.h>
#include <tdelocale.h>
#include <kdebug.h>


#include "main.moc"
#include "mapwidget.h"
#include "zoneclock.h"


WorldWideWatch::WorldWideWatch(bool restore, TQWidget *parent, const char *name)
  : TDEMainWindow(parent, name)
{
  TDEGlobal::locale()->insertCatalogue("timezones"); // For time zone translation

  TQWidget *w = new TQWidget(this);
  setCentralWidget(w);

  setPlainCaption(i18n("TDE World Clock"));
  
  TQVBoxLayout *vbox = new TQVBoxLayout(w, 0,0);
  
  _map = new MapWidget(false, restore, w);
  vbox->addWidget(_map,1);

  _clocks = new ZoneClockPanel(w);
  vbox->addWidget(_clocks);

  connect(_map, TQT_SIGNAL(addClockClicked(const TQString &)), _clocks, TQT_SLOT(addClock(const TQString &)));
  connect(_map, TQT_SIGNAL(saveSettings()), this, TQT_SLOT(doSave()));

  if (restore)
    load(kapp->config());
}


void WorldWideWatch::load(TDEConfig *config)
{
  _map->load(config);
  _clocks->load(config);

  resize(config->readNumEntry("Width", 320),
  config->readNumEntry("Height", 200));
}


void WorldWideWatch::save(TDEConfig *config)
{
  _map->save(config);
  _clocks->save(config);
  
  config->writeEntry("Width", width());
  config->writeEntry("Height", height());
}


void WorldWideWatch::doSave()
{
  save(kapp->config());
}


void WatchApplication::dumpMap()
{
  // guess some default parameters
  TQSize mapSize(kapp->desktop()->size());
 
  TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();
 
  TQCString themeName = args->getOption("theme");
  TQCString outName = args->getOption("o");
 
  TQCString ssize = args->getOption("size");
  if (!ssize.isEmpty())
    {
      int w,h;
      if (sscanf(ssize.data(), "%dx%d", &w, &h) == 2)
        mapSize = TQSize(w,h);
    }
 
  kdDebug() << "theme=" << themeName << " out=" << outName << " size=" << mapSize.width() << "x" << mapSize.height() << endl;

  MapWidget *w = new MapWidget(false, true, 0);
  w->setTheme(themeName);
  w->setSize(mapSize.width(), mapSize.height());

  TQPixmap pm = w->calculatePixmap();
  TQPainter p(&pm);
  w->paintContents(&p);
  pm.save(outName, "PPM");

  delete w;
}


int WatchApplication::newInstance()
{
  // dump mode, used for background drawing
  TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();
  if (args->isSet("dump"))
    {
      dumpMap();
      return 0;
    }

  if (!restoringSession())
  {
    WorldWideWatch *www = new WorldWideWatch(true);
    www->show();
  }

  return 0;
}

static void listThemes()
{
  TQPtrList<MapTheme> _themes = MapLoader::themes();
  TQPtrListIterator<MapTheme> it(_themes);
  for ( ; it.current(); ++it)
  {
     printf("%s\n", it.current()->tag().local8Bit().data());
  }
}

static TDECmdLineOptions options[] =
{
  { "dump", I18N_NOOP("Write out a file containing the actual map"), 0 },
  { "theme <file>", I18N_NOOP("The name of the theme to use"), "depths"},
  { "themes", I18N_NOOP("List available themes"), 0 },
  { "o <file>", I18N_NOOP("The name of the file to write to"), "dump.ppm" },
  { "size <WxH>", I18N_NOOP("The size of the map to dump"), 0 },
  TDECmdLineLastOption
};


int main(int argc, char *argv[])
{
  TDEAboutData about("kworldclock", I18N_NOOP("TDE World Clock"), "1.5");
  TDECmdLineArgs::init(argc, argv, &about);
  TDECmdLineArgs::addCmdLineOptions(options);
  KUniqueApplication::addCmdLineOptions();

  TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();
  if (args->isSet("themes"))
  {
    TDEInstance app(&about);
    listThemes();
    return 0;
  }

  if (!KUniqueApplication::start())
    return 0;

  WatchApplication app;
  if (args->isSet("dump"))
      app.disableSessionManagement();

  if (app.isRestored())
    RESTORE(WorldWideWatch)

  return app.exec();
}
