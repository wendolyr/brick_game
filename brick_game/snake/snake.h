/**
 * @file snake.h
 * @brief Header file for all snake backend functions
 */

#ifndef BRICKGAME_BRICK_GAME_SNAKE_SNAKE_H
#define BRICKGAME_BRICK_GAME_SNAKE_SNAKE_H

#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>

#include "../../headers/snake_defines.h"
#include "../../headers/specification.h"

namespace s21 {

/**
 * @brief Main class for snake game inplementation (model)
 */
class SnakeGame {
 protected:
  /**
   * @brief Describes a segment
   * @details x - x coordinate of the segment
   * @details y - y coordinate of the segment
   */
  struct Segment {
    int x;
    int y;
  };

  /**
   * @brief Describes possible directions of snake movement
   */
  enum class Direction { kUp, kDown, kLeft, kRight };

  /**
   * @brief Describes the possible states of finite state machine
   */
  enum class Fsm { kMenu, kSpawn, kMoving, kPauseMenu, kGameOver, kExit };

  /**
   * @brief Contains all information about the snake
   * @details body - deque of segments representing the snake's body
   * @details dir - current direction of movement
   * @details dir_lock - flag to prevent direction changes during frame
   */
  struct SnakeModel {
    std::deque<Segment> body;
    Direction dir;
    bool dir_lock;
  };

  /**
   * @brief Game timer class for controlling game speed
   */
  class GameTimer {
   public:
    /**
     * @brief Checks if timer has elapsed based on game speed
     * @param speed - current game speed
     * @param state - current game state
     * @return true if timer has elapsed, false otherwise
     */
    bool CheckTimer(int speed, SnakeGame::Fsm state);

   private:
    std::chrono::steady_clock::time_point last_time_;
    double time_accumilated_ = 0;
    bool initialized_ = false;
  };

  /**
   * @brief Checks if game timer has elapsed
   * @return true if timer has elapsed, false otherwise
   */
  bool Timer();

  /**
   * @brief Initializes snake body at starting position
   */
  void InitSnakeBody();

  /**
   * @brief Generates apple at random position
   */
  void RandomSpawnApple();

  /**
   * @brief Places snake segments on game field
   */
  void PlaceSnakeOnField();

  /**
   * @brief Places apple on game field
   */
  void PlaceAppleOnField();

  /**
   * @brief Clears the game field
   */
  void ClearField();

  /**
   * @brief Prepares game for new session
   */
  void PrepareForNewGame();

  /**
   * @brief Prepares game for exit
   */
  void PrepareForExit();

  /**
   * @brief Processes user input in main menu state
   * @param action - user input action
   */
  void MainMenuInteraction(UserAction_t action);

  /**
   * @brief Processes user input in pause menu state
   * @param action - user input action
   */
  void PauseMenuInteraction(UserAction_t action);

  /**
   * @brief Processes user input in game over state
   * @param action - user input action
   */
  void GameOverInteraction(UserAction_t action);

  /**
   * @brief Processes direction changes from user input
   * @param action - user input action
   */
  void DirectionProcessing(UserAction_t action);

  /**
   * @brief Moves snake according to current direction
   */
  void Moving();

  /**
   * @brief Checks for collisions with edges or self
   * @param head - head of the snake
   * @return true if collision detected, false otherwise
   */
  bool IsCollision(Segment head);

  /**
   * @brief Saves high score to file
   */
  void SaveHighScore();

  /**
   * @brief Loads high score from file
   */
  void GetHighScore();

 public:
  /**
   * @brief Constructor - initializes game state
   */
  SnakeGame();

  /**
   * @brief Destructor
   */
  ~SnakeGame();

  /**
   * @brief Processes user input
   * @param action - user input action
   * @param hold - whether the key is being held down
   */
  void Input(UserAction_t action, bool hold);

  /**
   * @brief Gets current game info
   * @return GameInfo_t struct with current game state
   */
  GameInfo_t CurrentState() const;

 protected:
  GameInfo_t info_;   ///< Current game information
  SnakeModel snake_;  ///< Current snake information
  Segment apple_;     ///< Current apple position
  Fsm state_;         ///< Current game state
};

/**
 * @brief Controller class for Snake game
 */
class SnakeController {
 private:
  SnakeController() = default;
  ~SnakeController() = default;

 public:
  SnakeController(const SnakeController &other) = delete;
  SnakeController(SnakeController &&other) = delete;

  /**
   * @brief Processes user input
   * @param action - user input action
   * @param hold - whether the key is being held down
   */
  static void userInput(UserAction_t action, bool hold) {
    game_.Input(action, hold);
  }

  /**
   * @brief Gets current game info
   * @return GameInfo_t struct with current game state
   */
  static GameInfo_t updateCurrentState() { return game_.CurrentState(); }

 private:
  inline static SnakeGame game_;  ///< Snake game instance
};

}  // namespace s21

#endif