cmake_minimum_required(VERSION 3.5)
project(xxSimulator VERSION 0.01 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        QCustomplot/qcustomplot.cpp
        QCustomplot/qcustomplot.h
        creatureBase.h creatureBase.cpp
        imagerenderer.h imagerenderer.cpp
        simumap.h simumap.cpp
        simumain.h simumain.cpp
        plantbase.h plantbase.cpp
        gene.h gene.cpp
        geneconn.h geneconn.cpp
        genome.h genome.cpp
        layer.h layer.cpp
        rulebook.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(xxSimulator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET xxSimulator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(xxSimulator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(xxSimulator
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(xxSimulator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(xxSimulator PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(xxSimulator)
endif()
