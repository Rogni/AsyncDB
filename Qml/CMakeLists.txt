cmake_minimum_required(VERSION 3.10)

project(QmlAsyncDB)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SOURCES
    Include/AsyncDB/QmlAsyncDB.h
    Src/AsyncDB/QmlAsyncDB.cpp
    )

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC Include/)

set (UsedQtComponents Core Qml)

find_package(Qt5 COMPONENTS
    ${UsedQtComponents}
    REQUIRED)

qt5_use_modules(${PROJECT_NAME} ${UsedQtComponents})

target_link_libraries(${PROJECT_NAME} AsyncDB)
