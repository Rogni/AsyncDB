cmake_minimum_required(VERSION 3.10)

project(CppTests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (TESTS
    ThreadTests.cpp
    )

set (UsedQtComponents Core Test)

find_package(Qt5 COMPONENTS
    ${UsedQtComponents}
    REQUIRED)

function(MakeTests File)

    get_filename_component(Filename ${File} NAME_WE)

    set (TestName ${PROJECT_NAME}_${Filename})

    add_executable(${TestName} ${File})

    qt5_use_modules(${TestName} ${UsedQtComponents})
    target_link_libraries(${TestName} AsyncDB)
    add_test(${TestName} ${TestName})
endfunction(MakeTests)

foreach(TestFile ${TESTS})
    MakeTests(${TestFile})
endforeach(TestFile)
