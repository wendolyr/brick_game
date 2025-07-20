/**
 * @file tetris_defines.h
 * @brief Header for common defines for back and front
 */

#ifndef BRICKGAME_HEADERS_TETRIS_DEFINES_H
#define BRICKGAME_HEADERS_TETRIS_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief height of the game field
 */
#define FIELD_HEIGHT 20

/**
 * @brief width of the game field
 */
#define FIELD_WIDTH 10

/**
 * @brief total tetromino height
 */
#define TETR_SIZE 5

/**
 * @brief Game pause states enumeration
 * @details Describes different menu and pause states
 */
typedef enum {
  kNoPause,      ///< Game is running normally
  kMainMenuOne,  ///< Main menu first option selected
  kMainMenuTwo,  ///< Main menu second option selected
  kPauseOne,     ///< Pause menu first option selected
  kPauseTwo,     ///< Pause menu second option selected
  kGameOverOne,  ///< Game over menu first option selected
  kGameOverTwo,  ///< Game over menu second option selected
} PauseT;

#ifdef __cplusplus
}
#endif

#endif