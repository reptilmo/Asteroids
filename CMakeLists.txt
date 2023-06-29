cmake_minimum_required(VERSION 3.23)

project(Asteroids)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_subdirectory(3rdparty/glad)
add_subdirectory(source)
