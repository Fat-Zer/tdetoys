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


##### kteatime (executable) #####################

tde_add_executable( kteatime AUTOMOC
  SOURCES main.cpp toplevel.cpp tealist.cpp
    timeedit.cpp
  LINK tdeio-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( kteatime )

tde_install_icons(
  DESTINATION ${DATA_INSTALL_DIR}/kteatime/icons )

##### other data ################################

install( FILES eventsrc
  DESTINATION ${DATA_INSTALL_DIR}/kteatime
)

install( FILES kteatime.desktop
  DESTINATION ${XDG_APPS_INSTALL_DIR}
)

