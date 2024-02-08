cmake_minimum_required(VERSION 3.12)

project(TriRender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

include_directories(${INCLUDE_DIR})

add_compile_options(-Wall -Wextra -pedantic -O3)
add_executable(render src/main.cpp src/AbstractObject.cpp src/MathUtils.cpp)
target_link_libraries(render sfml-graphics)