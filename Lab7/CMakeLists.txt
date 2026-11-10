cmake_minimum_required(VERSION 3.16)

project(Lab7 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Lab7
    main.cpp
    Lab7.h
    Lab7.cpp
    Lab7.ui
    ArchivoContactos.h
    ArchivoContactos.cpp
)

# Igual que en la Parte 1: se quitan los avisos de funciones obsoletas
target_compile_definitions(Lab7 PRIVATE QT_NO_DEPRECATED_WARNINGS)

target_link_libraries(Lab7 PRIVATE Qt6::Widgets)

set_target_properties(Lab7 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
