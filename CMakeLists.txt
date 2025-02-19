cmake_minimum_required(VERSION 3.10)
project(Holes VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
file(GLOB LIBRARIES "lib/*.cpp" "lib/**/*.cpp" "lib/*.c" "lib/**/*.c")

add_executable(Holes 
    ${SOURCES} 
    ${LIBRARIES} 
)

target_link_libraries(Holes PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

target_include_directories(Holes PRIVATE 
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS} 
    ${SDL2_TTF_INCLUDE_DIRS} 
    ${SDL2_MIXER_INCLUDE_DIRS}
    "include" "lib"
)
