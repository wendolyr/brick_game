/**
 * @file brick_game.h
 * @brief Main header file for BrickGame application
 * @details Contains entry points for both Tetris and Snake games
 */

#ifndef BRICKGAME_HEADERS_BRICK_GAME_H
#define BRICKGAME_HEADERS_BRICK_GAME_H

#include "../brick_game/snake/snake.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../brick_game/tetris/tetris.h"

#ifdef __cplusplus
}
#endif

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Console interface game loop
 * @param updateCurrentState - callback function to get current game state
 * @param userInput - callback function to handle user input
 */
void ConsoleInterface(GameInfo_t (*updateCurrentState)(),
                      void (*userInput)(UserAction_t, bool));

#ifdef __cplusplus
}
#endif

/**
 * @brief Desktop GUI interface entry point
 * @param updateCurrentState - callback function to get current game state
 * @param userInput - callback function to handle user input
 * @return application exit code
 */
int DesktopInterface(GameInfo_t (*updateCurrentState)(),
                     void (*userInput)(UserAction_t, bool));

#endif