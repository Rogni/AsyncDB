cmake_minimum_required(VERSION 3.10)

project(AsyncDbTests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SOURCES
    main.cpp
    qml.qrc
    )

set (RESOURCES
    TestsDB
    )

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RESOURCE "${RESOURCES}"
    )

set (UsedQtComponents Core Qml Gui)

find_package(Qt5 COMPONENTS
    ${UsedQtComponents}
    REQUIRED)

qt5_use_modules(${PROJECT_NAME} ${UsedQtComponents})

target_link_libraries(${PROJECT_NAME} QmlAsyncDB)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TestsDB.sqlite ${CMAKE_CURRENT_BINARY_DIR}/TestsDB.sqlite
)
