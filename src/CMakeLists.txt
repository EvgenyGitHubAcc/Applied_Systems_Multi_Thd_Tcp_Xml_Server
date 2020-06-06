add_executable(AppSysImpl main.cpp netlistener.cpp consolewriter.cpp mainthd.cpp)
target_link_libraries(AppSysImpl pugixml ws2_32)