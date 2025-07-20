#include "snake.h"

namespace s21 {

SnakeGame::SnakeGame() {
  state_ = Fsm::kMenu;
  info_.pause = static_cast<int>(PauseT::kMenuOne);
  info_.level = info_.speed = 1;
  info_.score = 0;
  GetHighScore();

  info_.field = new int *[kHeight];
  for (int i = 0; i < kHeight; ++i) {
    info_.field[i] = new int[kWidth]();
  }

  info_.next = nullptr;

  InitSnakeBody();
}

SnakeGame::~SnakeGame() {
  if (info_.field) {
    for (int i = 0; i < kHeight; ++i) {
      delete[] info_.field[i];
    }

    delete[] info_.field;
  }
}

void SnakeGame::Input(UserAction_t action, bool hold) {
  hold = hold;
  if (Timer() && state_ == Fsm::kMoving && action != Terminate) {
    Moving();
  } else if (state_ == Fsm::kMoving) {
    DirectionProcessing(action);
  } else if (state_ == Fsm::kMenu) {
    MainMenuInteraction(action);
  } else if (state_ == Fsm::kPauseMenu) {
    PauseMenuInteraction(action);
  } else if (state_ == Fsm::kGameOver) {
    GameOverInteraction(action);
  }

  PlaceSnakeOnField();

  if (state_ == Fsm::kSpawn) {
    RandomSpawnApple();
  } else if (state_ == Fsm::kExit) {
    PrepareForExit();
  }

  if (state_ == Fsm::kMoving) {
    PlaceAppleOnField();
  }
}

void SnakeGame::DirectionProcessing(UserAction_t action) {
  if (action == Terminate) {
    state_ = Fsm::kExit;
    return;
  } else if (action == Pause) {
    state_ = Fsm::kPauseMenu;
    info_.pause = static_cast<int>(PauseT::kPauseOne);
    return;
  }

  if (!snake_.dir_lock) {
    if (action == Left && snake_.dir != Direction::kRight) {
      snake_.dir = Direction::kLeft;
      snake_.dir_lock = true;
    } else if (action == Right && snake_.dir != Direction::kLeft) {
      snake_.dir = Direction::kRight;
      snake_.dir_lock = true;
    } else if (action == Up && snake_.dir != Direction::kDown) {
      snake_.dir = Direction::kUp;
      snake_.dir_lock = true;
    } else if (action == Down && snake_.dir != Direction::kUp) {
      snake_.dir = Direction::kDown;
      snake_.dir_lock = true;
    }
  }

  if (action == Action) {
    Moving();
  }
}

void SnakeGame::Moving() {
  ClearField();
  snake_.dir_lock = false;
  Segment head = snake_.body.front();
  if (snake_.dir == Direction::kRight) {
    head.x += 1;
  } else if (snake_.dir == Direction::kLeft) {
    head.x -= 1;
  } else if (snake_.dir == Direction::kUp) {
    head.y -= 1;
  } else {
    head.y += 1;
  }

  if (IsCollision(head)) {
    state_ = Fsm::kGameOver;
    info_.pause = static_cast<int>(PauseT::kGameOverOne);
  } else {
    if (head.x == apple_.x && head.y == apple_.y) {
      state_ = Fsm::kSpawn;
      ++info_.score;
      if (info_.score % kLevelUpScore == 0 && info_.level < kMaxLevel) {
        info_.level = 1 + info_.score / kLevelUpScore;
        info_.speed = info_.level;
      }
    } else {
      snake_.body.pop_back();
    }

    snake_.body.push_front(head);
  }
}

void SnakeGame::RandomSpawnApple() {
  if (snake_.body.size() == kHeight * kWidth) {
    state_ = Fsm::kGameOver;
    info_.pause = static_cast<int>(PauseT::kGameOverOne);
    info_.level = kMaxLevel + 1;
    return;
  }

  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<> dist_x(0, 9);
  std::uniform_int_distribution<> dist_y(0, 19);
  bool can_place = false;
  Segment new_apple = {0, 0};
  do {
    new_apple = {dist_x(gen), dist_y(gen)};
    if (!info_.field[new_apple.y][new_apple.x]) {
      can_place = true;
    }
  } while (!can_place);

  apple_ = new_apple;
  state_ = Fsm::kMoving;
}

GameInfo_t SnakeGame::CurrentState() const { return info_; }

}  // namespace s21