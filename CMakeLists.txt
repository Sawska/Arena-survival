cmake_minimum_required(VERSION 3.10)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(GameEngine
    main.cpp
    engine.cpp
    renderer.cpp
    player.cpp
    renderPlayer.cpp
    renderEnemy.cpp
    enemy.cpp
    spawner.cpp
    bullet.cpp
    renderBullet.cpp
    fast_enemy.cpp
    tank_enemy.cpp
    ranged_enemy.cpp
    renderObstacle.cpp
    Obstacle.cpp
    obstacleSpawner.cpp
    button.cpp
)

target_link_libraries(GameEngine
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:GameEngine>/assets
    COMMENT "Copying assets to build directory"
)