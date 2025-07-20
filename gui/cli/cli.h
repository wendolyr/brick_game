/**
 * @file cli.h
 * @brief Header file for all terminal frontend functions
 */

#ifndef BRICKGAME_GUI_CLI_CLI_H
#define BRICKGAME_GUI_CLI_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ncurses.h>

#include "../../headers/specification.h"
#include "../../headers/tetris_defines.h"

/*

SIZES

*/

/**
 * @brief total window height
 */
#define WINDOW_HEIGHT 24

/**
 * @brief total window width
 */
#define WINDOW_WIDTH 37

/**
 * @brief width of one cell
 */
#define CELL_WIDTH 2

/*

ACTION BUTTONS

*/

/**
 * @brief button to rotate tetromino
 */
#define KEY_SPACE ' '

/**
 * @brief button to pause
 */
#define KEY_PAUSE_2 'p'

/**
 * @brief CAPSLOCKed button to pause
 */
#define KEY_PAUSE_1 'P'

/**
 * @brief menu button to select an action
 */
#define KEY_START '\n'

/**
 * @brief button to exit the game
 */
#define KEY_ESC 27

/*

COORDINATES

*/

/**
 * @brief y coordinate of the first line of the field
 */
#define FIELD_TOP 2

/**
 * @brief y coordinate of the last line of the field
 */
#define FIELD_BOTTOM 21

/**
 * @brief x coordinate of the first column of the field
 */
#define FIELD_LEFT 3

/**
 * @brief x coordinate of the last column of the field
 */
#define FIELD_RIGHT 22

/**
 * @brief start x coordinate of the tetromino center
 */
#define TETR_SPAWN_X 13

/**
 * @brief start y coordinate of the tetromino center
 */
#define TETR_SPAWN_Y 2

/*

FUNCTIONS

*/

/**
 * @brief Turns on all ncurses settings
 */
void createWindow();

/**
 * @brief Player input processing
 * @return current user action
 */
UserAction_t inputProcessing();

/**
 * @brief Prints frames for window and field
 */
void printFrames();

/**
 * @brief Prints used buttons in the game
 */
void printControlTips();

/**
 * @brief Prints some tetromino in main menu and pause menu
 */
void printLogo();

/**
 * @brief Prints game screen
 * @param tetris - game params and field
 */
void gameDisplay(GameInfo_t *tetris);

/**
 * @brief Prints main menu screen
 * @param pause_screen - type of printing screen
 */
void printMainMenu(int pause_screen);

/**
 * @brief Prints pause menu screen
 * @param pause_screen - type of printing screen
 */
void printPauseMenu(int pause_screen);

/**
 * @brief Prints game over screen
 * @param tetris - game params
 */
void printGameOverScreen(GameInfo_t *tetris);

/**
 * @brief Clears game field and next tetromino preview when pause
 */
void clearGameInfo();

/**
 * @brief Prints game field on the screen
 * @param game - game info and field filling
 */
void printField(GameInfo_t *game);

#ifdef __cplusplus
}
#endif

#endif