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

##### check for gcc visibility support #########
# FIXME
# This should check for [T]Qt3 visibility support

if( WITH_GCC_VISIBILITY )
  if( NOT UNIX )
    tde_message_fatal(FATAL_ERROR "\ngcc visibility support was requested, but your system is not *NIX" )
  endif( NOT UNIX )
  set( __KDE_HAVE_GCC_VISIBILITY 1 )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif( WITH_GCC_VISIBILITY )


tde_setup_architecture_flags( )

# required stuff
find_package( TQt )
find_package( TDE )
