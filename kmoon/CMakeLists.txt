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


##### kmoon_panelapplet (kpart) #################

tde_add_kpart( kmoon_panelapplet AUTOMOC
  SOURCES kmoonapplet.cpp phases.cpp kmoondlg.cpp
    kmoonwidget.cpp
  LINK tdeui-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( kmoon )


##### other data ################################

install( FILES kmoonapplet.desktop
  DESTINATION ${DATA_INSTALL_DIR}/kicker/applets
)

