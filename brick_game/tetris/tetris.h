/**
 * @file tetris.h
 * @brief Header file for all tetris backend functions
 */

#ifndef BRICKGAME_BRICK_GAME_TETRIS_TETRIS_H
#define BRICKGAME_BRICK_GAME_TETRIS_TETRIS_H

#ifdef __cplusplus
extern "C" {
#endif

// #ifdef __linux__
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
// #endif

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "../../headers/specification.h"
#include "../../headers/tetris_defines.h"
#include "matrix.h"

/*

DEFINES

*/

/**
 * @brief max level of the game
 */
#define MAX_LEVEL 10

/**
 * @brief required points to level up
 */
#define LEVELUP_SCORE 600

/**
 * @brief points for one destroyed row
 */
#define SCORE_ONE 100

/**
 * @brief points for two destroyed rows
 */
#define SCORE_TWO 300

/**
 * @brief points for three destroyed rows
 */
#define SCORE_THREE 700

/**
 * @brief points for four destroyed rows
 */
#define SCORE_FOUR 1500

/**
 * @brief directory for storing the game record
 */
#define HIGH_SCORE_DIR "./records/.tetris_record.txt"

/*

TYPES

*/

/**
 * @brief Describes the possible states of finite state machine
 */
typedef enum {
  kMainMenu,
  kSpawn,
  kMoving,
  kPauseMenu,
  kAttaching,
  kGameOver,
  kExit,
} Fsm;

/**
 * @brief Describes the information about tetromino
 * @details shape - pointer at the tetromino TETR_HEIGHT*TETR_WIDTH
 * @details index - type of tetromino from 1 to 7
 * @details rotation_stage - current stage of rotation
 * @details y - tetromino center y coordinate
 * @details x - tetromino center x coordinate
 */
typedef struct {
  int **shape;
  int index;
  int rotation_stage;
  int y;
  int x;
} Tetromino;

/**
 * @brief Describes the all information about the game
 * @details tetris - information about game field
 * @details tetromino - information about current tetromino
 * @details state - current state of the game
 */
typedef struct {
  GameInfo_t *tetris;
  Tetromino *tetromino;
  Fsm state;
} Singleton;

/*

FSM functions

*/

/**
 * @brief sets one of the form tetromino
 * @param color - generated color
 * @param type - generated type of tetromino
 * @param shape - tetromino shape matrix
 */
void settingTetromino(int color, int type, int **shape);

/**
 * @brief generates and spawns tetrominoes
 * @param game - pointer to struct that contains tetromino struct
 */
void spawningTetromino(Singleton *game);

/**
 * @brief processing actions in moving state
 * @param action - current player action
 * @param hold - key hold tracking (not used)
 */
void movingProcessing(UserAction_t action, bool hold);

/**
 * @brief moves current tetromino one pixel down
 * @param game - pointer to all information about the game
 * @return 0 if can move down and 1 otherwise
 */
int movingDown(Singleton *game);

/**
 * @brief moves current tetromino one pixel left
 * @param game - pointer to all information about the game
 */
void movingLeft(Singleton *game);

/**
 * @brief moves current tetromino one pixel right
 * @param game - pointer to all information about the game
 */
void movingRight(Singleton *game);

/**
 * @brief rotate current tetromino
 * @param game - pointer to all information about the game
 */
void rotatingTetromino(Singleton *game);

/**
 * @brief checks for collision current tetromino with field
 * @param game - pointer to all information about the game
 * @param permission - pointer to variable that tracks the total possibility
 * @return 0 if there are no collision and 1 otherwise
 */
int collisionCheck(Singleton *game);

/**
 * @brief attaches tetriminoes to the field when can't move down
 * @param game - pointer to all information about the game
 * @return 0 if can attach tetromino and 1 if can't
 */
int attachingTetromino(Singleton *game);

/**
 * @brief checks if there are any filled lines after attaching tetromino
 * @param game - pointer to all information about the game
 */
void checkingFieldRows(Singleton *game);

/**
 * @brief destroys filled line on the field
 * @param tetris - pointer to current field info
 * @param row - line that needs to be destroyed
 */
void destroyRow(GameInfo_t *tetris, int row);

/**
 * @brief scoring if there are any destroyed lines
 * @param game - pointer to all information about the game
 * @param destroyed_rows - amount of destroyed lines
 */
void scoring(Singleton *game, int destroyed_rows);

/*

MENU INTERACTION FUNCTIONS

*/

/**
 * @brief buttons press processing in main menu
 * @param game - pointer to all information about the game
 * @param action - current player pressed button
 */
void mainMenuInteraction(Singleton *game, UserAction_t action);

/**
 * @brief buttons press processing in game over menu
 * @param game - pointer to all information about the game
 * @param action - current player pressed button
 */
void gameOverInteraction(Singleton *game, UserAction_t action);

/**
 * @brief button press processing in pause menu
 * @param game - pointer to all information about the game
 * @param action - current player pressed button
 */
void pauseInteraction(Singleton *game, UserAction_t action);

/*

SPECIFICATION FUNCTIONS

*/

/**
 * @brief user input processing
 * @param action - current player pressed button
 * @param hold - holding button (true only when pressing down)
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief update information about game
 * @return struct that contains current params of the game
 */
GameInfo_t updateCurrentState();

/*

OTHER BACKEND FUNCTIONS

*/

/**
 * @brief place tetromino on field matrix
 * @param tetris - current state of the game
 * @param tetromino - current state of the tetromino
 */
void mergeFieldWithFigure(GameInfo_t *tetris, Tetromino *tetromino);

/**
 * @brief checks the possibility to place ghost tetromino on field
 * @param tetris - current state of the game
 * @param tetromino - current state of the tetromino
 * @param i - i-th row of the tetromino matrix
 * @param j - j-th column of the tetromino matrix
 * @param k - shift relative to the tetromino
 * @param flag - tracking the possibility to place ghost tetromino
 */
void checkGhostPlace(GameInfo_t *tetris, Tetromino *tetromino, int i, int j,
                     int k, int *flag);

/**
 * @brief gets access to struct Singleton
 * @return pointer to Singleton struct
 */
Singleton **getState();

/**
 * allocates memory for GameInfo_t struct
 * @return ST_OK if memory allocated successfully and ST_ERROR otherwise
 */
int createGameInfo(GameInfo_t **tetris);

/**
 * allocates memory for Tetromino struct
 * @return ST_OK if memory allocated successfully and ST_ERROR otherwise
 */
int createTetromino(Tetromino **tetromino);

/**
 * @brief clear the field and reset all stats
 */
void prepareForNewGame();

/**
 * @brief frees all allocated memory
 */
void removeGame();

/**
 * @brief get high score from the file or set 0 if there is no such file
 * @param tetris - current game info
 */
void getHighScore(GameInfo_t *tetris);

/**
 * @brief save current score to the file if it's greater than the record
 * @param tetris - current game info
 */
void saveHighScore(GameInfo_t *tetris);

/**
 * @brief set timer depends of current speed
 * @return 1 if timer is up and 0 otherwise
 */
int timer();

#ifdef __cplusplus
}
#endif

#endif