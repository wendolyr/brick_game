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
 * @details field - pointer at the game field matrix HEIGHT*WIDTH
 * @details next - pointer at the next tetromino that will be spawned
 * @details score - player score
 * @details high_score - local game record
 * @details level - current game level from 1 to 10
 * @details speed - current game speed
 * @details pause - pause state for menu interaction
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#ifdef __cplusplus
}
#endif

#endif