#include "snake.h"

namespace s21 {

void SnakeGame::MainMenuInteraction(UserAction_t action) {
  if (action == Terminate) {
    state_ = Fsm::kExit;
    return;
  }

  if (info_.pause == static_cast<int>(PauseT::kMenuOne)) {
    if (action == Down)
      info_.pause = static_cast<int>(PauseT::kMenuTwo);
    else if (action == Start) {
      state_ = Fsm::kSpawn;
      info_.pause = static_cast<int>(PauseT::kNoPause);
    }
  } else {
    if (action == Up)
      info_.pause = static_cast<int>(PauseT::kMenuOne);
    else if (action == Start)
      state_ = Fsm::kExit;
  }
}

void SnakeGame::PauseMenuInteraction(UserAction_t action) {
  if (action == Terminate) {
    state_ = Fsm::kExit;
    return;
  }

  if (action == Pause) {
    state_ = Fsm::kMoving;
    info_.pause = static_cast<int>(PauseT::kNoPause);
  }

  if (info_.pause == static_cast<int>(PauseT::kPauseOne)) {
    if (action == Start) {
      state_ = Fsm::kMoving;
      info_.pause = static_cast<int>(PauseT::kNoPause);
    } else if (action == Down)
      info_.pause = static_cast<int>(PauseT::kPauseTwo);
  } else {
    if (action == Start) {
      state_ = Fsm::kExit;
    } else if (action == Up)
      info_.pause = static_cast<int>(PauseT::kPauseOne);
  }
}

void SnakeGame::GameOverInteraction(UserAction_t action) {
  if (action == Terminate) {
    state_ = Fsm::kExit;
    return;
  }

  if (info_.pause == static_cast<int>(PauseT::kGameOverOne)) {
    if (action == Start) {
      PrepareForNewGame();
    } else if (action == Down)
      info_.pause = static_cast<int>(PauseT::kGameOverTwo);

  } else {
    if (action == Start) {
      state_ = Fsm::kExit;
    } else if (action == Up)
      info_.pause = static_cast<int>(PauseT::kGameOverOne);
  }
}

}  // namespace s21