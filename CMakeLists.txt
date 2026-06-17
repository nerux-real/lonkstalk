cmake_minimum_required(VERSION 3.16)

project(VLBeats VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VLBeats
    core/main.cpp
    core/Game.cpp
    core/Window.cpp
    gameplay/Beatmap.cpp
    gameplay/ParticleSystem.cpp
    gameplay/Skins.cpp
    deps/miniz.c
    deps/sqlite3.c
    # utils/DiscordRPC.cpp
    utils/Hash.cpp
    utils/Database.cpp
    utils/DebugOverlay.cpp
    utils/Settings.cpp
)
configure_file(${CMAKE_SOURCE_DIR}/app_icon.png ${CMAKE_BINARY_DIR}/app_icon.png COPYONLY)

add_executable(LKPacker
    tools/LKPacker/lkpacker.cpp
    deps/miniz.c
)

add_executable(LKEditor
    tools/LKEditor/lkeditor.cpp
    tools/LKEditor/deps/imgui_demo.cpp
    tools/LKEditor/deps/imgui_draw.cpp
    tools/LKEditor/deps/imgui_tables.cpp
    tools/LKEditor/deps/imgui_widgets.cpp
    tools/LKEditor/deps/imgui.cpp
    tools/LKEditor/deps/backends/imgui_impl_sdl2.cpp
    tools/LKEditor/deps/backends/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(LKEditor PRIVATE
    tools/LKEditor/deps
    tools/LKEditor/deps/backends
)
target_link_libraries(LKEditor PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

find_package(SDL2_gfx CONFIG QUIET)

target_link_libraries(VLBeats PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

if(NOT WIN32)
    target_link_libraries(VLBeats PRIVATE pthread dl)
endif()

if(APPLE)
    target_include_directories(VLBeats PRIVATE /opt/homebrew/include /usr/local/include)
    set(MACOSX_BUNDLE_ICON_FILE "app_icon.icns")
    set_source_files_properties(${CMAKE_SOURCE_DIR}/app_icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(VLBeats PRIVATE ${CMAKE_SOURCE_DIR}/app_icon.icns)
    target_include_directories(LKPacker PRIVATE /opt/homebrew/include /usr/local/include)
endif()

if(UNIX AND NOT APPLE)
    install(FILES lonkstalk.desktop DESTINATION share/applications)
    install(FILES app_icon.png DESTINATION share/icons/hicolor/128x128/apps)
endif()

if(TARGET SDL2_gfx::SDL2_gfx)
    target_link_libraries(VLBeats PRIVATE SDL2_gfx::SDL2_gfx)
endif()

target_compile_features(LKPacker PRIVATE cxx_std_17)

if(WIN32)
    target_compile_definitions(VLBeats PRIVATE _CRT_SECURE_NO_WARNINGS)
    set(APP_ICON_RESOURCE "${CMAKE_BINARY_DIR}/app_icon.rc")
    file(WRITE ${APP_ICON_RESOURCE} "IDI_ICON1 ICON \"${CMAKE_SOURCE_DIR}/app_icon.ico\"")
    target_sources(VLBeats PRIVATE ${APP_ICON_RESOURCE})
endif()