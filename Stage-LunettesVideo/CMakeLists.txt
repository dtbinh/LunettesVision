cmake_minimum_required(VERSION 2.8)
set (CMAKE_MODULE_PATH .) 
project(LunettesVideo)

add_definitions(-std=c++11)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} /usr/local/include/ )

find_package( uEyeUsb REQUIRED )
include_directories( ${UEYEUSB_INCLUDE_DIR} )
message("Found uEye : " ${UEYEUSB_INCLUDE_DIR})

include_directories( src )
aux_source_directory( src sourcefiles )

add_executable( LunettesVideo ${sourcefiles} ) 
target_link_libraries( LunettesVideo ${OpenCV_LIBS} ${UEYEUSB_LIBRARY})

