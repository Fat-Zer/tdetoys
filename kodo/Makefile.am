
# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

bin_PROGRAMS = kodo
kodo_SOURCES = main.cpp kodometer.cpp kimagenumber.cpp
METASOURCES = AUTO

# the library search path.
kodo_LDFLAGS = $(all_libraries) $(KDE_RPATH) $(LIB_QT) -lDCOP $(LIB_TDECORE) $(LIB_TDEUI) -ltdefx $(LIB_TDEIO) -ltdetexteditor
kodo_LDADD   = $(LIB_TDEUI)

messages:
	$(XGETTEXT) $(kodo_SOURCES) -o $(podir)/kodo.pot

KDE_ICON = kodo

xdg_apps_DATA = kodo.desktop

pics_DATA = numbers.png
picsdir = $(kde_datadir)/kodo

