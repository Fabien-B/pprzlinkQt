cmake_minimum_required(VERSION 3.5)

project(PprzlinkQtExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Network Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS  Core Widgets Network Xml REQUIRED)

set(PprzlinkQt_DIR "../PprzlinkQt/")
add_subdirectory(${PprzlinkQt_DIR} pprzlinkqt)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PprzlinkQtExample ${PROJECT_SOURCES})
else()
    add_executable(PprzlinkQtExample ${PROJECT_SOURCES})
endif()


target_include_directories(PprzlinkQtExample PRIVATE ${PprzlinkQt_DIR})

target_link_libraries(PprzlinkQtExample PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    PprzlinkQt
    IvyQt
)
