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

##### other data ################################

install( FILES blobrc ghostrc eyesrc bonhommerc
  nekorc pingurc wormrc tips-en billyrc bsdrc
  tuxrc taorc
  DESTINATION ${DATA_INSTALL_DIR}/amor
)

tde_install_symlink( tips-en ${DATA_INSTALL_DIR}/amor/tips )
