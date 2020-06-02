cmake_minimum_required(VERSION 3.5)

project(AppSysImpl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Libs/pugixml-1.10)
include_directories("Libs/pugixml-1.10/src")

#add_definitions(-DDEBUG_XML)

add_executable(AppSysImpl main.cpp netlistener.cpp clientmgr.cpp consolewriter.cpp thd.cpp)

target_link_libraries(AppSysImpl pugixml)
