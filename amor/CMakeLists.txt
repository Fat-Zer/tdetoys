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

add_subdirectory( data )

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


##### amor (executable) #########################

tde_add_executable( amor AUTOMOC
  SOURCES amordialog.cpp amor.cpp amorwidget.cpp
    main.cpp amoranim.cpp amorpm.cpp
    amorbubble.cpp amorconfig.cpp amortips.cpp
    AmorIface.skel
  LINK kio-shared Xext
  DESTINATION ${BIN_INSTALL_DIR}
)


##### includes ##################################

install( FILES AmorIface.h
  DESTINATION ${INCLUDE_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( amor )


##### other data ################################

install( FILES amor.desktop
  DESTINATION ${XDG_APPS_INSTALL_DIR}
)

