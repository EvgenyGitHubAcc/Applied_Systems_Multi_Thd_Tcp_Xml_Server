cmake_minimum_required(VERSION 3.5)

project(AppSysImpl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Libs/pugixml-1.10)
include_directories("Libs/pugixml-1.10/src")

#add_definitions(-DCONS_WRITE_DEBUG)

add_executable(AppSysImpl main.cpp netlistener.cpp consolewriter.cpp mainthd.cpp clientmgr.cpp)

target_link_libraries(AppSysImpl pugixml ws2_32)
