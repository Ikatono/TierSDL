cmake_minimum_required(VERSION 3.21)
project(tiersdl)

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" ON)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_subdirectory(vendored/SDL_ttf)
add_subdirectory(vendored/SDL_image)

include_directories(include)
#include_directories(vendored/SDL_ttf/include)

# Create your game executable target as usual
add_executable(tiersdl WIN32
	src/main.cpp
    src/window.cpp
    src/mouse.cpp
    src/tiercell.cpp
    src/tierrow.cpp
    src/tierboard.cpp
    src/text.cpp
    src/graphics.cpp
    )

# Link to the actual SDL3 library.
target_link_libraries(tiersdl PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    )

