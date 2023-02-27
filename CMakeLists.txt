
cmake_minimum_required(VERSION 3.8)
project( main_Robot )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( class.cpp )
target_link_libraries( main_Robot ${OpenCV_LIBS} )
