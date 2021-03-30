cmake_minimum_required(VERSION 3.10)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets LinguistTools Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets LinguistTools Xml REQUIRED)

file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
message("Read qrc:" ${QRC_FILES})

file(GLOB_RECURSE SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
message("Read src:" ${SOURCE_LIST})

message("Read ts:" ${TS_FILES})

set(PROJECT_SOURCES
    ${SOURCE_LIST}
    ${QRC_FILES}
    ${TS_FILES}
)

set(CPP_COMMON_LIB
    logger
    util
)

set(QT_LIB_LIST
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Widgets
)

set(APP_LIB
    kkcommon
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${CMAKE_PROJECT_NAME}
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${CMAKE_PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${CMAKE_PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CPP_COMMON_LIB} ${QT_LIB_LIST} ${APP_LIB})
