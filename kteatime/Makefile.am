
INCLUDES= $(all_includes)

bin_PROGRAMS = kteatime

kteatime_SOURCES = main.cpp toplevel.cpp tealist.cpp timeedit.cpp

kteatime_METASOURCES = AUTO
kteatime_LDFLAGS = $(all_libraries) $(KDE_RPATH) $(LIB_QT) -lDCOP $(LIB_TDECORE) $(LIB_TDEUI) -ltdefx $(LIB_TDEIO) -ltdetexteditor

# LIB_TDEIO needed for KNotifyDialog
kteatime_LDADD = $(LIB_TDEUI) $(LIB_TDEIO)

KDE_ICON = kteatime

KDE_OPTIONS = nofinal

appsdata_DATA = eventsrc
appsdatadir = $(kde_datadir)/kteatime

kteatimeicondir = $(kde_datadir)/kteatime/icons
kteatimeicon_ICON = AUTO

messages:
	$(XGETTEXT) $(kteatime_SOURCES) -o $(podir)/kteatime.pot

xdg_apps_DATA = kteatime.desktop
