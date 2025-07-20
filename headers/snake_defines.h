/**
 * @file snake_defines.h
 * @brief Header with constants and enums for Snake game
 */
#ifndef BRICKGAME_HEADERS_SNAKE_DEFINES_H
#define BRICKGAME_HEADERS_SNAKE_DEFINES_H

namespace s21 {

/**
 * @brief Width of game field in cells
 */
constexpr int kWidth = 10;

/**
 * @brief Height of game field in cells
 */
constexpr int kHeight = 20;

/**
 * @brief Size of next figure
 */
constexpr int kNext = 5;

/**
 * @brief Color index for snake segments
 */
constexpr int kSnakeColor = 3;

/**
 * @brief Color index for apples
 */
constexpr int kAppleColor = 1;

/**
 * @brief Maximum game level
 */
constexpr int kMaxLevel = 10;

/**
 * @brief Score required to level up
 */
constexpr int kLevelUpScore = 5;

/**
 * @brief Game pause states enumeration
 * @details Describes different menu and pause states
 */
enum class PauseT {
  kNoPause,      ///< Game is running normally
  kMenuOne,      ///< Main menu first option selected
  kMenuTwo,      ///< Main menu second option selected
  kPauseOne,     ///< Pause menu first option selected
  kPauseTwo,     ///< Pause menu second option selected
  kGameOverOne,  ///< Game over menu first option selected
  kGameOverTwo   ///< Game over menu second option selected
};

}  // namespace s21

#endif