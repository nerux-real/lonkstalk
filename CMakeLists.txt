cmake_minimum_required(VERSION 3.16)

project(Lonkstalk VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lonkstalk
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
    SDL2::SDL2
    SDL2::SDL2main
)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

find_package(SDL2_gfx CONFIG QUIET)

target_link_libraries(Lonkstalk PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

if(NOT WIN32)
    target_link_libraries(Lonkstalk PRIVATE pthread dl)
endif()

if(APPLE)
    target_include_directories(Lonkstalk PRIVATE /opt/homebrew/include /usr/local/include)
    target_include_directories(LKPacker PRIVATE /opt/homebrew/include /usr/local/include)
endif()

if(TARGET SDL2_gfx::SDL2_gfx)
    target_link_libraries(Lonkstalk PRIVATE SDL2_gfx::SDL2_gfx)
endif()

# find_library(DISCORD_RPC_LIB
#     NAMES discord-rpc
# )

# if(DISCORD_RPC_LIB)
#     target_link_libraries(Lonkstalk PRIVATE ${DISCORD_RPC_LIB})
# endif()

target_compile_features(LKPacker PRIVATE cxx_std_17)

if(WIN32)
    target_compile_definitions(Lonkstalk PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()