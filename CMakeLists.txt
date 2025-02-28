cmake_minimum_required(VERSION 3.10)
project(Holes VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
file(GLOB SDL2_GFX "lib/SDL2_gfx/*.c")
add_library(SDL2_gfx ${SDL2_GFX})
target_link_libraries(SDL2_gfx PRIVATE SDL2::SDL2)

if(APPLE)
    set_source_files_properties(platform/mac/error.mm PROPERTIES COMPILE_FLAGS "-ObjC++")
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
endif()

file(GLOB SOURCES "src/*.cpp" "src/*.cc" "src/*.c")
file(GLOB LIBRARIES "lib/*.cpp" "lib/**/*.cpp" "lib/*.c" "lib/**/*.c")

if (APPLE)
add_executable(Holes 
    ${SOURCES}
    platform/mac/error.mm
    ${LIBRARIES}
)
else()
add_executable(Holes
    ${SOURCES}
    platform/other/error.cc
    ${LIBRARIES}
)
endif()

if(APPLE)
  target_link_libraries(Holes SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf ${COCOA_FRAMEWORK} SDL2_gfx)
else()
  target_link_libraries(Holes PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_gfx)
endif()

target_include_directories(Holes PRIVATE 
    "include" "lib"
)
