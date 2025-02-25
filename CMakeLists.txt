cmake_minimum_required(VERSION 3.16)

project(init LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(loglib REQUIRED)
find_package(SettingsLib REQUIRED)

add_executable(init main.cpp
    inittask.h inittask.cpp
    inittaskparser.h

    inittaskcontainer.h inittaskcontainer.cpp
    utils.h
    inittaskherder.h inittaskherder.cpp)

target_link_libraries(init SettingsLib loglib)

include(GNUInstallDirs)
install(TARGETS init
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
