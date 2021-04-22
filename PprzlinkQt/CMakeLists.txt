cmake_minimum_required(VERSION 3.14)

project(PprzlinkQt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Xml
    Network
    REQUIRED)

set(IvyQt_DIR "../IvyQt/IvyQt/")
add_subdirectory(${IvyQt_DIR} ivyqt)

add_library(PprzlinkQt STATIC
    pprzlinkqt.cpp
    message_definition.cpp
    message.cpp
)

target_include_directories(PprzlinkQt PRIVATE ${IvyQt_DIR})

#set_target_properties(PprzlinkQt PROPERTIES
#    VERSION 0.0.1
#)

target_link_libraries(PprzlinkQt PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Network
)


target_compile_definitions(PprzlinkQt PRIVATE PPRZLINKQT_LIBRARY)


install(TARGETS PprzlinkQt DESTINATION lib)
install(FILES
    pprzlinkqt.h
    message_definition.h
    message.h
    DESTINATION include)





