cmake_minimum_required(VERSION 3.25)
project(32fp2hdr)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "${HOME}/Qt/5.12.2/gcc_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(32fp2hdr src/main.cpp
        src/filesystem.cpp
        src/filesystem.h
        src/filesystem.h
        src/MainWindow.cpp
        src/MainWindow.h)

target_link_libraries(32fp2hdr
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

