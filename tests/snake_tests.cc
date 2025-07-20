#include <gtest/gtest.h>

#include <thread>

#include "../brick_game/snake/snake.h"

class SnakeTest : public s21::SnakeGame {
 public:
  using SnakeGame::apple_;
  using SnakeGame::info_;
  using SnakeGame::snake_;
  using SnakeGame::state_;

 public:
  using SnakeGame::Direction;
  using SnakeGame::Fsm;
  using SnakeGame::Segment;
  using SnakeGame::SnakeModel;

 public:
  using SnakeGame::ClearField;
  using SnakeGame::DirectionProcessing;
  using SnakeGame::GameOverInteraction;
  using SnakeGame::GetHighScore;
  using SnakeGame::InitSnakeBody;
  using SnakeGame::IsCollision;
  using SnakeGame::MainMenuInteraction;
  using SnakeGame::Moving;
  using SnakeGame::PauseMenuInteraction;
  using SnakeGame::PlaceAppleOnField;
  using SnakeGame::PlaceSnakeOnField;
  using SnakeGame::PrepareForExit;
  using SnakeGame::PrepareForNewGame;
  using SnakeGame::RandomSpawnApple;
  using SnakeGame::SaveHighScore;
  using SnakeGame::Timer;
};

TEST(SnakeTest, GetHighScore) {
  SnakeTest test;
  test.GetHighScore();
  EXPECT_EQ(test.info_.high_score, 0);

  std::ofstream file("./records/.snake_record.txt");
  file << "Hello";
  test.GetHighScore();
  EXPECT_EQ(test.info_.high_score, 0);

  test.info_.score = 1;
  test.SaveHighScore();
  test.GetHighScore();
  EXPECT_EQ(test.info_.high_score, 1);
}

TEST(SnakeTest, InitSnakeBodyTest) {
  SnakeTest test;
  EXPECT_EQ(test.snake_.body.size(), 4);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kRight);
  EXPECT_FALSE(test.snake_.dir_lock);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMenu);
}

TEST(SnakeTest, RandomSpawnAppleTest) {
  SnakeTest test;
  test.apple_ = {22, 22};
  test.RandomSpawnApple();

  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);
  SnakeTest::Segment t{22, 22};
  EXPECT_NE(test.apple_.x, t.x);
  EXPECT_NE(test.apple_.y, t.y);

  test.snake_.body.clear();
  test.ClearField();
  test.state_ = SnakeTest::Fsm::kSpawn;
  for (int i = 0; i < s21::kHeight; ++i) {
    for (int j = 0; j < s21::kWidth; ++j) {
      if (i != s21::kHeight - 1 || j != s21::kWidth - 1) {
        t = {j, i};
        test.snake_.body.push_back(t);
      }
    }
  }

  test.PlaceSnakeOnField();
  test.RandomSpawnApple();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  test.snake_.body.clear();
  test.ClearField();
  for (int i = 0; i < s21::kHeight; ++i) {
    for (int j = 0; j < s21::kWidth; ++j) {
      t = {j, i};
      test.snake_.body.push_back(t);
    }
  }
  test.PlaceSnakeOnField();
  test.RandomSpawnApple();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kGameOver);
}

TEST(SnakeTest, PlaceSnakeOnField) {
  SnakeTest test;
  test.PlaceSnakeOnField();
  EXPECT_EQ(test.info_.field[s21::kHeight / 2][s21::kWidth / 2 - 2],
            s21::kSnakeColor);
  EXPECT_EQ(test.info_.field[s21::kHeight / 2][s21::kWidth / 2 - 1],
            s21::kSnakeColor);
  EXPECT_EQ(test.info_.field[s21::kHeight / 2][s21::kWidth / 2],
            s21::kSnakeColor);
  EXPECT_EQ(test.info_.field[s21::kHeight / 2][s21::kWidth / 2 + 1],
            s21::kSnakeColor);
}

TEST(SnakeTest, PlaceAppleOnField) {
  SnakeTest test;
  test.apple_ = {3, 3};
  test.PlaceAppleOnField();
  EXPECT_EQ(test.info_.field[3][3], s21::kAppleColor);
}

TEST(SnakeTest, ClearField) {
  SnakeTest test;
  test.RandomSpawnApple();
  test.PlaceSnakeOnField();
  test.ClearField();
  for (int i = 0; i < s21::kHeight; ++i) {
    for (int j = 0; j < s21::kWidth; ++j) {
      EXPECT_EQ(test.info_.field[i][j], 0);
    }
  }
}

TEST(SnakeTest, PrepareForNewGame) {
  SnakeTest test;
  test.RandomSpawnApple();
  test.info_.level = test.info_.speed = 3;
  test.info_.high_score = 0;
  test.info_.pause = static_cast<int>(s21::PauseT::kGameOverOne);
  test.SaveHighScore();
  test.info_.score = 25;
  test.PrepareForNewGame();
  EXPECT_EQ(test.info_.level, 1);
  EXPECT_EQ(test.info_.speed, 1);
  EXPECT_EQ(test.info_.score, 0);
  EXPECT_EQ(test.info_.high_score, 25);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kNoPause));
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kSpawn);
  test.PrepareForNewGame();
}

TEST(SnakeTest, PrepareForExit) {
  SnakeTest test;
  test.info_.high_score = 0;
  test.SaveHighScore();
  test.info_.score = 10;
  test.PrepareForExit();

  EXPECT_EQ(test.info_.field, nullptr);
  test.PrepareForExit();

  SnakeTest test2;
  test2.PrepareForExit();
  EXPECT_EQ(test2.info_.field, nullptr);
}

TEST(SnakeTest, Timer) {
  SnakeTest test;
  test.state_ = SnakeTest::Fsm::kMoving;
  EXPECT_FALSE(test.Timer());
  EXPECT_FALSE(test.Timer());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(test.Timer());
  test.state_ = SnakeTest::Fsm::kPauseMenu;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  EXPECT_FALSE(test.Timer());
}

TEST(SnakeTest, IsCollision) {
  SnakeTest test;
  SnakeTest::Segment t = {-1, 1};
  EXPECT_TRUE(test.IsCollision(t));
  t = {1, -1};
  EXPECT_TRUE(test.IsCollision(t));
  t = {1, s21::kHeight};
  EXPECT_TRUE(test.IsCollision(t));
  t = {s21::kWidth, 1};
  EXPECT_TRUE(test.IsCollision(t));
  t = {1, 1};
  EXPECT_FALSE(test.IsCollision(t));
  test.snake_.body.push_back(t);
  EXPECT_TRUE(test.IsCollision(t));
  t = {2, 2};
  EXPECT_FALSE(test.IsCollision(t));
  t = {1, 2};
  EXPECT_FALSE(test.IsCollision(t));
  t = {2, 1};
  EXPECT_FALSE(test.IsCollision(t));
}

TEST(SnakeTest, DirectionProcessing) {
  SnakeTest test;
  test.RandomSpawnApple();
  test.DirectionProcessing(Terminate);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);
  test.state_ = SnakeTest::Fsm::kMoving;
  test.DirectionProcessing(Pause);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kPauseMenu);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kPauseOne));
  test.state_ = SnakeTest::Fsm::kMoving;
  test.DirectionProcessing(Left);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kRight);
  EXPECT_FALSE(test.snake_.dir_lock);
  test.DirectionProcessing(Up);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kUp);
  EXPECT_TRUE(test.snake_.dir_lock);
  test.DirectionProcessing(Left);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kUp);
  test.snake_.dir_lock = false;
  test.DirectionProcessing(Down);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kUp);
  EXPECT_FALSE(test.snake_.dir_lock);
  test.DirectionProcessing(Left);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kLeft);
  EXPECT_TRUE(test.snake_.dir_lock);
  test.snake_.dir_lock = false;
  test.DirectionProcessing(Right);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kLeft);
  EXPECT_FALSE(test.snake_.dir_lock);
  test.DirectionProcessing(Down);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kDown);
  EXPECT_TRUE(test.snake_.dir_lock);
  test.snake_.dir_lock = false;
  test.DirectionProcessing(Up);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kDown);
  EXPECT_FALSE(test.snake_.dir_lock);
  test.DirectionProcessing(Right);
  EXPECT_EQ(test.snake_.dir, SnakeTest::Direction::kRight);
  EXPECT_TRUE(test.snake_.dir_lock);
  test.DirectionProcessing(Action);
}

TEST(SnakeTest, Moving) {
  SnakeTest test;
  test.apple_ = {s21::kWidth / 2 + 2, s21::kHeight / 2};
  test.state_ = SnakeTest::Fsm::kMoving;
  test.Moving();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kSpawn);

  test.apple_ = {s21::kWidth / 2 + 3, s21::kHeight / 2};
  test.info_.score = s21::kLevelUpScore - 1;
  test.state_ = SnakeTest::Fsm::kMoving;
  test.Moving();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kSpawn);
  EXPECT_EQ(test.info_.level, 2);

  test.apple_ = {s21::kWidth / 2 + 4, s21::kHeight / 2};
  test.info_.score = s21::kLevelUpScore * s21::kMaxLevel + 4;
  test.info_.level = s21::kMaxLevel;
  test.state_ = SnakeTest::Fsm::kMoving;
  test.Moving();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kSpawn);
  EXPECT_EQ(test.info_.level, 10);

  test.apple_ = {s21::kWidth / 2 + 4, 1};
  test.state_ = SnakeTest::Fsm::kMoving;

  test.snake_.dir = SnakeTest::Direction::kUp;
  test.Moving();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  test.snake_.dir = SnakeTest::Direction::kLeft;
  test.Moving();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  test.snake_.dir = SnakeTest::Direction::kDown;
  test.Moving();
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kGameOver);
}

TEST(SnakeTest, Input) {
  SnakeTest test;
  test.Input(None, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMenu);

  test.Input(Start, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  test.Input(Pause, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kPauseMenu);

  test.Input(Pause, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  std::this_thread::sleep_for(std::chrono::seconds(1));
  test.Input(None, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  while (test.state_ != SnakeTest::Fsm::kGameOver) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    test.Input(None, false);
  }
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kGameOver);

  test.Input(Start, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  std::this_thread::sleep_for(std::chrono::seconds(1));
  test.Input(Terminate, false);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);
}

TEST(SnakeTest, CurrentState) {
  SnakeTest test;
  EXPECT_EQ(test.info_.field, test.CurrentState().field);
  EXPECT_EQ(test.info_.next, test.CurrentState().next);
}

TEST(SnakeTest, MainMenuInteraction) {
  SnakeTest test;
  test.MainMenuInteraction(Terminate);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);

  test.state_ = SnakeTest::Fsm::kMenu;
  test.MainMenuInteraction(Down);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kMenuTwo));

  test.MainMenuInteraction(Up);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kMenuOne));

  test.MainMenuInteraction(Start);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kNoPause));
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kSpawn);

  test.info_.pause = static_cast<int>(s21::PauseT::kMenuTwo);
  test.MainMenuInteraction(Start);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);

  test.MainMenuInteraction(Down);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kMenuTwo));
}

TEST(SnakeTest, PauseMenuInteraction) {
  SnakeTest test;
  test.info_.pause = static_cast<int>(s21::PauseT::kPauseOne);

  test.PauseMenuInteraction(Terminate);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);

  test.state_ = SnakeTest::Fsm::kPauseMenu;
  test.PauseMenuInteraction(Down);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kPauseTwo));

  test.PauseMenuInteraction(Up);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kPauseOne));

  test.PauseMenuInteraction(Up);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kPauseOne));

  test.PauseMenuInteraction(Start);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kNoPause));
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kMoving);

  test.info_.pause = static_cast<int>(s21::PauseT::kPauseTwo);
  test.PauseMenuInteraction(Start);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);

  test.PauseMenuInteraction(Down);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kPauseTwo));
}

TEST(SnakeTest, GameOverInteraction) {
  SnakeTest test;
  test.RandomSpawnApple();
  test.info_.pause = static_cast<int>(s21::PauseT::kGameOverOne);

  test.GameOverInteraction(Terminate);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);

  test.state_ = SnakeTest::Fsm::kGameOver;
  test.GameOverInteraction(Down);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kGameOverTwo));

  test.GameOverInteraction(Up);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kGameOverOne));

  test.GameOverInteraction(Up);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kGameOverOne));

  test.GameOverInteraction(Start);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kNoPause));
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kSpawn);

  test.info_.pause = static_cast<int>(s21::PauseT::kGameOverTwo);
  test.GameOverInteraction(Start);
  EXPECT_EQ(test.state_, SnakeTest::Fsm::kExit);

  test.GameOverInteraction(Down);
  EXPECT_EQ(test.info_.pause, static_cast<int>(s21::PauseT::kGameOverTwo));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}