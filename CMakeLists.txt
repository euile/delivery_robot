cmake_minimum_required(VERSION 3.0)
project( robot )

find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)

find_package(json-c CONFIG)

find_package( OpenCV REQUIRED)
find_package( TgBot REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} ${TgBot_INCLUDE_DIRS} )
add_executable( robot main.cpp route.h route.cpp tgbot.h tgbot.cpp)
target_link_libraries( robot PkgConfig::Mosquitto json-c::json-c ${OpenCV_LIBS} ${TgBot_LIBS})
