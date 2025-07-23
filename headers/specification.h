/**
 * @file specification.h
 * @brief Header file for specification types for BrickGame
 */

#ifndef BRICKGAME_HEADERS_SPECIFICATION_H
#define BRICKGAME_HEADERS_SPECIFICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Describes the possible actions of the  player when pressing the
 * buttons
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  None
} UserAction_t;

/**
 * @brief Describes the information about game field
 */
typedef struct {
  int **field;     ///< pointer at the game field matrix HEIGHT*WIDTH
  int **next;      ///< pointer at the next tetromino that will be spawned
  int score;       ///< player score
  int high_score;  ///< local game record
  int level;       ///< current game level from 1 to 10
  int speed;       ///< current game speed
  int pause;       ///< pause state for menu interaction
} GameInfo_t;

#ifdef __cplusplus
}
#endif

#endif