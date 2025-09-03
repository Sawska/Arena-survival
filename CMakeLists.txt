cmake_minimum_required(VERSION 3.10)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)


link_directories(${SDL2_LIBRARY_DIRS})



add_executable(GameEngine engine.cpp renderer.cpp  main.cpp)

target_include_directories(GameEngine PRIVATE ${SDL2_INCLUDE_DIRS})


target_link_libraries(GameEngine ${SDL2_LIBRARIES})