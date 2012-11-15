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

cmake_minimum_required( VERSION 2.8 )


##### general package setup #####################

project( kdeutils )
set( VERSION 3.5.13.1 )


##### include essential cmake modules ###########

include( FindPkgConfig )
include( CheckIncludeFile )
include( CheckLibraryExists )
include( CheckCSourceCompiles )
include( CheckCXXSourceCompiles )
# @Add othere required cmake modules here@
#
# EXAMPLE:
# include( CheckTypeSize )


##### include our cmake modules #################

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
include( TDEMacros )


##### setup install paths #######################

include( TDESetupPaths )
tde_setup_paths( )


##### optional stuff ############################

# @Add configuration options here@
#
# EXAMPLE:
# option( WITH_T1LIB "Enable t1lib support" OFF )


##### options comments ##########################

# @Add configuration options descriptions here@
# NOTE: They should be commented.
# EXAMPLE:
# # WITH_T1LIB   affects <modules which affects this variable>
# # WITH_T1LIB   description <a verbose description what functionality is
#                            addeded or removed by this option>


##### user requested modules ####################

option( BUILD_ALL           "Build all" OFF )
option( BUILD_AMOR          "Build amor" ${BUILD_ALL} )
option( BUILD_DOC           "Build doc" ${BUILD_ALL} )
option( BUILD_EYESAPPLET    "Build eyesapplet" ${BUILD_ALL} )
option( BUILD_FIFTEENAPPLET "Build fifteenapplet" ${BUILD_ALL} )
option( BUILD_KMOON         "Build kmoon" ${BUILD_ALL} )
option( BUILD_KODO          "Build kodo" ${BUILD_ALL} )
option( BUILD_KTEATIME      "Build kteatime" ${BUILD_ALL} )
option( BUILD_KTUX          "Build ktux" ${BUILD_ALL} )
option( BUILD_KWEATHER      "Build kweather" ${BUILD_ALL} )
option( BUILD_KWORLDWATCH   "Build kworldwatch" ${BUILD_ALL} )


##### configure checks ##########################

include( ConfigureChecks.cmake )


###### global compiler settings #################

add_definitions(
  -DHAVE_CONFIG_H
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TQT_CXX_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined" )
set( CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined" )


##### @TDE_MODULE_NAME@  directories ############

tde_conditional_add_subdirectory( BUILD_AMOR          amor )
tde_conditional_add_subdirectory( BUILD_DOC           doc )
tde_conditional_add_subdirectory( BUILD_EYESAPPLET    eyesapplet )
tde_conditional_add_subdirectory( BUILD_FIFTEENAPPLET fifteenapplet )
tde_conditional_add_subdirectory( BUILD_KMOON         kmoon )
tde_conditional_add_subdirectory( BUILD_KODO          kodo )
tde_conditional_add_subdirectory( BUILD_KTEATIME      kteatime )
tde_conditional_add_subdirectory( BUILD_KTUX          ktux )
tde_conditional_add_subdirectory( BUILD_KWEATHER      kweather )
tde_conditional_add_subdirectory( BUILD_KWORLDWATCH   kworldwatch )


##### write configure files #####################

configure_file( config.h.cmake config.h @ONLY )
