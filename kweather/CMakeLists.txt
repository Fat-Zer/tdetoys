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

add_subdirectory( graphics )

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


##### kweatherreport (tdeinit) ##################

tde_add_tdeinit_executable( kweatherreport AUTOMOC
  SOURCES reportmain.cpp weatherservice.stub
    reportview.cpp
  LINK tdeui-shared tdehtml-shared
)


##### kweatherservice (executable) ##############

tde_add_executable( kweatherservice AUTOMOC
  SOURCES main.cpp metar_parser.cpp
    weatherlib.cpp weatherservice.cpp
    weatherservice.skel stationdatabase.cpp
    sun.cpp weatherservice.stub
  LINK tdeio-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### sun_test (test) ###########################
# FIXME: add correct test targets support
# 
# tde_add_executable( sun_test AUTOMOC
#   SOURCES sun_test.cpp sun.cpp
# )
# 
# add_test( NAME sun_test-test
#   COMMAND sun_test)
# 
# 
##### stationdatabase_test (test) ###############
# 
# tde_add_executable( stationdatabase_test AUTOMOC
#   SOURCES stationdatabase_test.cpp
#     stationdatabase.cpp
# )
# 
# add_test( NAME stationdatabase_test-test
#   COMMAND stationdatabase_test)
# 
# 
##### metar_parser_test (test) ##################
# 
# tde_add_executable( metar_parser_test AUTOMOC
#   SOURCES metar_parser_test.cpp metar_parser.cpp
#     stationdatabase.cpp sun.cpp
# )
# 
# add_test( NAME metar_parser_test-test
#   COMMAND metar_parser_test)
# 

##### weather_panelapplet (kpart) ###############

tde_add_kpart( weather_panelapplet AUTOMOC
  SOURCES reportview.cpp kweather.cpp
    dockwidget.cpp weatherIface.skel
    weatherservice.stub weatherbutton.cpp
  LINK tdehtml-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### kcm_weather (kpart) #######################

tde_add_kpart( kcm_weather AUTOMOC
  SOURCES kcmweather.cpp prefdialogdata.ui
    weatherservice.stub
  LINK tdeio-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### kcm_weatherservice (kpart) ################

tde_add_kpart( kcm_weatherservice AUTOMOC
  SOURCES kcmweatherservice.cpp
    serviceconfigwidget.cpp serviceconfigdata.ui
    weatherservice.stub
  LINK tdeui-shared tdehtml-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### icons #####################################

tde_install_icons( kweather )


##### other data ################################

install( FILES weather_stations.desktop
  DESTINATION ${DATA_INSTALL_DIR}/kweatherservice
)

install( FILES stations.dat
  DESTINATION ${DATA_INSTALL_DIR}/kweatherservice
)

install( FILES kweather.desktop
  DESTINATION ${DATA_INSTALL_DIR}/kicker/applets
)

install(
  FILES
    kweatherservice.desktop
    kcmweather.desktop
    kcmweatherservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
)

