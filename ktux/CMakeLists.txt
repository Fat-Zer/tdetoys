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

add_subdirectory( sprites )

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


##### ktux (executable) #########################

tde_add_executable( ktux AUTOMOC
  SOURCES spriteanim.cpp spritemisc.cpp
    spritepm.cpp sprite.cpp
  LINK kio-shared kscreensaver-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( ktux )


##### other data ################################

install( FILES ktux.desktop
  DESTINATION
    ${APPS_INSTALL_DIR}/System/ScreenSavers
)

