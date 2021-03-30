cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME util)

file(GLOB_RECURSE SOURCE_LIST ./*.cpp)

add_library(${TARGET_NAME} STATIC
    ${SOURCE_LIST}
)

#target_link_libraries(${TARGET_NAME} PRIVATE )
