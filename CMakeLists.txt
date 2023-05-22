cmake_minimum_required(VERSION 3.0)
project( robot )

find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)

find_package(json-c CONFIG)

find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( robot tg.cpp main.cpp )
target_link_libraries( main PkgConfig::Mosquitto json-c::json-c ${OpenCV_LIBS} )
