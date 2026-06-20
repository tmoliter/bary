cmake_minimum_required(VERSION 3.21)
project(bary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---------------------------------------------------------------------------
# Dependency discovery
#
# macOS:   brew install cmake sdl2 sdl2_image sdl2_mixer lua
#
# Windows: install vcpkg, then:
#          vcpkg install sdl2 sdl2-image sdl2-mixer lua
#          and pass -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
#          to cmake when configuring.
# ---------------------------------------------------------------------------

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH /opt/homebrew)
endif()

find_package(SDL2       REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Lua        REQUIRED)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

set(SOURCES
    barysystem.cpp
    Camera.cpp
    input.cpp
    inventory.cpp
    jukebox.cpp
    luaUtils.cpp
    main.cpp
    Ray.cpp
    resourceDepository.cpp
    Scene.cpp
    Task.cpp
    Timer.cpp
    utils.cpp

    components/Animator.cpp
    components/Collidable.cpp
    components/Move.cpp
    components/Obstruction.cpp
    components/Sprite.cpp

    things/FieldPlayer.cpp
    things/RealThing.cpp

    gui/Line.cpp
    gui/MenuDisplay.cpp
    gui/Phrase.cpp
    gui/Text.cpp
    gui/UIRenderer.cpp

    editor/CommandLine.cpp
    editor/EventEditor.cpp
    editor/MapBuilder.cpp
    editor/RayEditor.cpp
    editor/SpriteEditor.cpp
    editor/ThingEditor.cpp
    editor/ThingRouter.cpp
)

# ---------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------

# WIN32 uses the WinMain entry point on Windows (no console window) and is
# a no-op on every other platform.
add_executable(bary WIN32 ${SOURCES})

target_include_directories(bary PRIVATE
    ${CMAKE_SOURCE_DIR}   # project headers: #include "Scene.h", "globals.h", etc.
    ${LUA_INCLUDE_DIR}    # lua.h, lualib.h, lauxlib.h
)

target_link_libraries(bary PRIVATE
    # SDL2main must precede SDL2 on Windows; the generator expression is a no-op elsewhere
    $<$<PLATFORM_ID:Windows>:SDL2::SDL2main>
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    ${LUA_LIBRARIES}
)

# ---------------------------------------------------------------------------
# Windows: copy required DLLs next to the executable after each build
# (TARGET_RUNTIME_DLLS requires CMake 3.21+)
# ---------------------------------------------------------------------------

if(WIN32)
    add_custom_command(TARGET bary POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:bary>
            $<TARGET_FILE_DIR:bary>
        COMMAND_EXPAND_LISTS
    )
endif()

# ---------------------------------------------------------------------------
# Note: the game loads assets via relative paths (scripts/load.lua, etc.)
# so run the binary from the project root:
#
#   cmake -B build && cmake --build build && ./build/bary
# ---------------------------------------------------------------------------
