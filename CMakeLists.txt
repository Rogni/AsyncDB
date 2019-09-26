cmake_minimum_required(VERSION 3.10)

project(AsyncDB)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SOURCES
    Include/Threads/ThreadManager.h
    Src/Threads/ThreadManager.cpp

    Include/AsyncDB/ADBDatabaseConfiguration.h
    Src/AsyncDB/ADBDatabaseConfiguration.cpp

    Include/AsyncDB/ADBDatabase.h
    Src/AsyncDB/ADBDatabase.cpp

    Include/AsyncDB/ADBListModel.h
    Src/AsyncDB/ADBListModel.cpp

    Src/AsyncDB/ADBError.h
    Src/AsyncDB/ADBError.cpp

    Include/AsyncDB/ADBListModelConfiguration.h
    Src/AsyncDB/ADBListModelConfiguration.cpp

    Include/AsyncDB/ADBAbstractListModelConfiguration.h
    Src/AsyncDB/ADBAbstractListModelConfiguration.cpp

    Include/AsyncDB/ADBItemModel.h
    Src/AsyncDB/ADBItemModel.cpp

    Include/AsyncDB/ADBCustomListModelConfiguration.h
    Src/AsyncDB/ADBCustomListModelConfiguration.cpp

    Include/AsyncDB/AsyncDB.h
    Src/AsyncDB/AsyncDB.cpp
    )

add_library(${PROJECT_NAME} ${SOURCES})

set (UsedQtComponents Core Sql)

find_package(Qt5 COMPONENTS
    ${UsedQtComponents}
    REQUIRED)

qt5_use_modules(${PROJECT_NAME} ${UsedQtComponents})

target_include_directories(${PROJECT_NAME} PUBLIC Include)

add_subdirectory(Qml)
add_subdirectory(Tests)
