cmake_minimum_required(VERSION 3.5)

project(IvyQtExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Widgets
    Network
    Xml
    REQUIRED)

set(IvyQt_DIR "IvyQt/IvyQt/")
add_subdirectory(${IvyQt_DIR} ivyqt)

set(PROJECT_SOURCES
        main.cpp
        pprzlinkqt.cpp
        message_definition.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(IvyQtExample ${PROJECT_SOURCES})
else()
    add_executable(IvyQtExample ${PROJECT_SOURCES})
endif()


target_include_directories(IvyQtExample PRIVATE ${IvyQt_DIR})

target_link_libraries(IvyQtExample PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    IvyQt
)
