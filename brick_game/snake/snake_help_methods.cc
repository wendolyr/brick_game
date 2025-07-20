#include "snake.h"

namespace s21 {

void SnakeGame::InitSnakeBody() {
  snake_.body.push_back({kWidth / 2 + 1, kHeight / 2});
  snake_.body.push_back({kWidth / 2, kHeight / 2});
  snake_.body.push_back({kWidth / 2 - 1, kHeight / 2});
  snake_.body.push_back({kWidth / 2 - 2, kHeight / 2});
  snake_.dir = Direction::kRight;
  snake_.dir_lock = false;
}

void SnakeGame::PlaceSnakeOnField() {
  for (const Segment &i : snake_.body) {
    info_.field[i.y][i.x] = kSnakeColor;
  }
}

void SnakeGame::PlaceAppleOnField() {
  info_.field[apple_.y][apple_.x] = kAppleColor;
}

void SnakeGame::ClearField() {
  for (const Segment &i : snake_.body) {
    info_.field[i.y][i.x] = 0;
  }

  info_.field[apple_.y][apple_.x] = 0;
}

void SnakeGame::PrepareForNewGame() {
  if (info_.score > info_.high_score) {
    SaveHighScore();
    info_.high_score = info_.score;
  }

  ClearField();
  snake_.body.clear();

  InitSnakeBody();
  info_.level = info_.speed = 1;
  info_.score = info_.pause = static_cast<int>(PauseT::kNoPause);
  state_ = Fsm::kSpawn;
}

void SnakeGame::PrepareForExit() {
  if (info_.field) {
    for (int i = 0; i < kHeight; ++i) {
      delete[] info_.field[i];
    }

    delete[] info_.field;
    info_.field = nullptr;
  }

  if (info_.score > info_.high_score) {
    SaveHighScore();
  }
}

bool SnakeGame::Timer() {
  static GameTimer timer;

  return timer.CheckTimer(info_.speed, state_);
}

bool SnakeGame::IsCollision(Segment head) {
  if (head.x < 0 || head.x >= kWidth || head.y < 0 || head.y >= kHeight) {
    return true;
  }

  for (const Segment &i : snake_.body) {
    if (i.x == head.x && i.y == head.y) {
      return true;
    }
  }

  return false;
}

void SnakeGame::SaveHighScore() {
  std::ofstream file("./records/.snake_record.txt");
  file << info_.score;

  file.close();
}

void SnakeGame::GetHighScore() {
  std::ifstream file("./records/.snake_record.txt");
  if (!file.is_open()) {
    info_.high_score = 0;
    return;
  }

  if (!(file >> info_.high_score)) {
    info_.high_score = 0;
  }

  info_.high_score = info_.high_score < 0 ? 0 : info_.high_score;

  file.close();
}

bool SnakeGame::GameTimer::CheckTimer(int speed, SnakeGame::Fsm state) {
  auto now = std::chrono::steady_clock::now();

  if (!initialized_) {
    last_time_ = now;
    initialized_ = true;
    return false;
  }
  if (state == SnakeGame::Fsm::kMoving) {
    auto elapsed = now - last_time_;
    last_time_ = now;

    time_accumilated_ += std::chrono::duration<double>(elapsed).count();

    const double waiting_time = 1 - 0.08 * speed;
    if (time_accumilated_ > waiting_time) {
      time_accumilated_ -= waiting_time;
      return true;
    }
  } else {
    last_time_ = now;
  }
  return false;
}

}  // namespace s21