cmake_minimum_required(VERSION 3.16)

project(Lonkstalk VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lonkstalk
    main.cpp
    Game.cpp
    Window.cpp
    Beatmap.cpp
    DebugOverlay.cpp
    ParticleSystem.cpp
    Settings.cpp
    Skins.cpp
    # DiscordRPC.cpp
    miniz.c
    sqlite3.c
    Hash.cpp
    Database.cpp
)

add_executable(LKPacker
    lkpacker.cpp
    miniz.c
)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

find_package(SDL2_gfx CONFIG QUIET)

target_link_libraries(Lonkstalk PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    pthread
    dl
)

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