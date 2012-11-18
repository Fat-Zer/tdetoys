#################################################
#
#  (C) 2012 Golubev Alexander
#  fatzer2 (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

add_subdirectory( maps )
add_subdirectory( pics )

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### ww (static) ###############################

tde_add_library( ww STATIC_PIC AUTOMOC
  SOURCES maploader.cpp mapwidget.cpp astro.c
    sunclock.c about.ui cities.cpp zoneclock.cpp
    flags.cpp flow.cpp clock.ui
)


##### kworldclock (executable) ##################

tde_add_executable( kworldclock AUTOMOC
  SOURCES main.cpp
  LINK kio-shared ww-static
  DESTINATION ${BIN_INSTALL_DIR}
)


##### ww_panelapplet (kpart) ####################

tde_add_kpart( ww_panelapplet AUTOMOC
  SOURCES applet.cpp
  LINK kio-shared ww-static
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( kworldclock )


##### other data ################################

install( FILES kworldclock.desktop
  DESTINATION ${XDG_APPS_INSTALL_DIR}
)

install( FILES kdeworld.desktop
  DESTINATION
    ${DATA_INSTALL_DIR}/kdesktop/programs
)

install( FILES zone.tab
  DESTINATION ${DATA_INSTALL_DIR}/kworldclock
)

install( FILES kwwapplet.desktop
  DESTINATION ${DATA_INSTALL_DIR}/kicker/applets
)

