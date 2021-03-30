cmake_minimum_required(VERSION 3.10)

project(kktools LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_HOME_DIRECTORY}
    ${CMAKE_HOME_DIRECTORY}/app
    ${CMAKE_HOME_DIRECTORY}/lib/logger
)

add_subdirectory(lib)
add_subdirectory(app)
