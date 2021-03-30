cmake_minimum_required(VERSION 3.10)

set(CMAKE_AUTOUIC ON)

set(TARGET_NAME kkcommon)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets LinguistTools REQUIRED)

file(GLOB_RECURSE SOURCE_LIST *.cpp)

file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
message("Read qrc:" ${QRC_FILES})

add_library(${TARGET_NAME}
    ${SOURCE_LIST} ${QRC_FILES}
)

target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets logger)
