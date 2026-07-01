cmake_minimum_required(VERSION 3.21)
project(bary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# BARY_DEV (default ON): on macOS, symlink the asset directories into the
# .app bundle instead of copying them, so editing Lua/assets in the repo takes
# effect live without a rebuild. Turn OFF for a self-contained, shareable
# bundle: cmake -B build -DBARY_DEV=OFF
option(BARY_DEV "Dev build: symlink assets into the bundle for live editing" ON)

# On non-Apple platforms the binary is a plain executable that reads assets
# from the directory next to it, so drop it in the project root.
#
# On macOS the assets live inside the .app bundle (Contents/Resources), so
# SDL_GetBasePath() finds them wherever the bundle sits. We deliberately leave
# the bundle in its own build tree (build/bary.app, build-release/bary.app,
# ...) so a dev (symlinked) and a shareable (copied) build never clobber each
# other, and the path you launch tells you exactly which one you're running.
if(NOT APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
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
    GameController.cpp
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
# macOS: build as a proper .app bundle.
#
# SDL_GetBasePath() returns Contents/Resources/ when running from a bundle,
# which is where Settings::init() chdir()s — so assets go there.
#
# Run with:  open bary.app
# ---------------------------------------------------------------------------
if(APPLE)
    set(MACOSX_ICON base/assets/cmake/bary.icns)
    set_source_files_properties(${MACOSX_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    set_target_properties(bary PROPERTIES
        MACOSX_BUNDLE                      TRUE
        MACOSX_BUNDLE_BUNDLE_NAME          "Bary"
        MACOSX_BUNDLE_GUI_IDENTIFIER       "com.bary.game"
        MACOSX_BUNDLE_BUNDLE_VERSION       "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_ICON_FILE            "bary.icns"
    )

    target_sources(bary PRIVATE ${MACOSX_ICON})

    # Get asset directories into Contents/Resources/ after each build.
    # If you ever add a new top-level asset directory, add it to this list.
    #
    # BARY_DEV: symlink each dir so repo edits (Lua, sprites) are live with no
    #           rebuild. The rm first clears any real dir left by a release
    #           build, since you can't symlink over an existing directory.
    # Release:  copy each dir so the bundle is self-contained and shareable.
    foreach(asset_dir base games scripts state config)
        if(BARY_DEV)
            add_custom_command(TARGET bary POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E rm -rf
                    $<TARGET_BUNDLE_CONTENT_DIR:bary>/Resources/${asset_dir}
                COMMAND ${CMAKE_COMMAND} -E create_symlink
                    ${CMAKE_SOURCE_DIR}/${asset_dir}
                    $<TARGET_BUNDLE_CONTENT_DIR:bary>/Resources/${asset_dir}
            )
        else()
            add_custom_command(TARGET bary POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E rm -rf
                    $<TARGET_BUNDLE_CONTENT_DIR:bary>/Resources/${asset_dir}
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/${asset_dir}
                    $<TARGET_BUNDLE_CONTENT_DIR:bary>/Resources/${asset_dir}
            )
        endif()
    endforeach()
endif()

# ---------------------------------------------------------------------------
# Dev build (macOS) scratch build: cmake -B build && cmake --build build
# Dev build (macOS) build:         cmake --build build;
#   Run: ./build/bary.app/Contents/MacOS/bary
#   The bundle symlinks back to the repo, so Lua/asset edits are live with no
#   rebuild. Running the inner binary keeps stdout attached (unlike `open`).
#
# Shareable build (macOS): cmake -B build-release -DBARY_DEV=OFF && cmake --build build-release
#   Run: ./build-release/bary.app/Contents/MacOS/bary
#   Copies assets into the bundle so it's self-contained. A separate build tree
#   from the dev one, so the two never clobber each other.
#
# Run (Windows): ./bary
# ---------------------------------------------------------------------------
