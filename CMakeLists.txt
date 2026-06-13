cmake_minimum_required(VERSION 3.16)
project(Lonkstalk)
set(CMAKE_CXX_STANDARD 17)

add_executable(Lonkstalk
    main.cpp
    Game.cpp
    Window.cpp
    Beatmap.cpp
    DebugOverlay.cpp
    miniz.c
    ParticleSystem.cpp
    Settings.cpp
)
add_executable(LKPacker
    lkpacker.cpp
    miniz.c
)

find_package(SDL2 REQUIRED)
target_include_directories(Lonkstalk PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Lonkstalk PRIVATE ${SDL2_LIBRARIES})
find_package(SDL2_mixer REQUIRED)
target_link_libraries(Lonkstalk PRIVATE SDL2_mixer)
target_link_libraries(Lonkstalk PRIVATE SDL2_gfx)
target_link_libraries(Lonkstalk PRIVATE SDL2_ttf)
target_link_libraries(Lonkstalk PRIVATE SDL2_image)
target_compile_features(LKPacker PRIVATE cxx_std_17)