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


##### kodo (executable) #########################

tde_add_executable( kodo AUTOMOC
  SOURCES main.cpp kodometer.cpp kimagenumber.cpp
  LINK tdeio-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( kodo )


##### other data ################################

install( FILES numbers.png
  DESTINATION ${DATA_INSTALL_DIR}/kodo
)

install( FILES kodo.desktop
  DESTINATION ${XDG_APPS_INSTALL_DIR}
)

