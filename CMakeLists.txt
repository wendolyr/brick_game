cmake_minimum_required(VERSION 3.5)
project(brick_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Curses REQUIRED)

add_executable(brick_game 
    brick_game.cc
    brick_game/snake/snake.cc
    brick_game/snake/snake_menu_interaction.cc
    brick_game/snake/snake_help_methods.cc
    brick_game/tetris/backend.c
    brick_game/tetris/fsm.c
    brick_game/tetris/matrix.c
    brick_game/tetris/menu_interactions.c
    gui/cli/cli.c
    gui/desktop/desktop.cc
)

add_compile_options(-Wall -Werror -Wextra)

target_include_directories(brick_game PRIVATE ${CURSES_INCLUDE_DIR})

target_link_libraries(brick_game
    PRIVATE 
    ${CURSES_LIBRARIES}
    Qt6::Widgets
)

set_target_properties(brick_game PROPERTIES
    AUTOMOC ON
)