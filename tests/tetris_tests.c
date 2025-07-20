#include <check.h>

#include "../brick_game/tetris/tetris.h"

#define TEST_BLOCKS 4

START_TEST(mainMenuInteractionTest) {
  Singleton *game = *getState();
  game->state = kMainMenu;
  game->tetris->pause = kMainMenuOne;
  mainMenuInteraction(game, Terminate);
  ck_assert_int_eq(game->state, kExit);

  game->state = kMainMenu;
  game->tetris->pause = kMainMenuOne;
  mainMenuInteraction(game, Down);
  ck_assert_int_eq(game->tetris->pause, kMainMenuTwo);

  mainMenuInteraction(game, Up);
  ck_assert_int_eq(game->tetris->pause, kMainMenuOne);

  mainMenuInteraction(game, Left);
  ck_assert_int_eq(game->tetris->pause, kMainMenuOne);

  mainMenuInteraction(game, Start);
  ck_assert_int_eq(game->tetris->pause, kNoPause);
  ck_assert_int_eq(game->state, kSpawn);

  game->state = kMainMenu;
  game->tetris->pause = kMainMenuTwo;

  mainMenuInteraction(game, Left);
  ck_assert_int_eq(game->tetris->pause, kMainMenuTwo);

  mainMenuInteraction(game, Start);
  ck_assert_int_eq(game->state, kExit);
}
END_TEST

START_TEST(pauseInteractionTest) {
  Singleton *game = *getState();

  game->state = kPauseMenu;
  game->tetris->pause = kPauseOne;
  pauseInteraction(game, Terminate);
  ck_assert_int_eq(game->state, kExit);

  game->state = kPauseMenu;
  pauseInteraction(game, Down);
  ck_assert_int_eq(game->tetris->pause, kPauseTwo);

  pauseInteraction(game, Up);
  ck_assert_int_eq(game->tetris->pause, kPauseOne);

  pauseInteraction(game, Left);
  ck_assert_int_eq(game->tetris->pause, kPauseOne);

  pauseInteraction(game, Start);
  ck_assert_int_eq(game->state, kMoving);
  ck_assert_int_eq(game->tetris->pause, 0);

  game->state = kPauseMenu;
  game->tetris->pause = kPauseTwo;
  pauseInteraction(game, Left);
  ck_assert_int_eq(game->tetris->pause, kPauseTwo);

  pauseInteraction(game, Start);
  ck_assert_int_eq(game->state, kExit);

  game->state = kPauseMenu;
  pauseInteraction(game, Pause);
  ck_assert_int_eq(game->state, kMoving);
}
END_TEST

START_TEST(gameOverInteractionTest) {
  Singleton *game = *getState();
  game->state = kGameOver;
  game->tetris->pause = kGameOverOne;

  gameOverInteraction(game, Terminate);
  ck_assert_int_eq(game->state, kExit);

  game->state = kGameOver;
  gameOverInteraction(game, Down);
  ck_assert_int_eq(game->tetris->pause, kGameOverTwo);
  ck_assert_int_eq(game->state, kGameOver);

  gameOverInteraction(game, Up);
  ck_assert_int_eq(game->tetris->pause, kGameOverOne);
  ck_assert_int_eq(game->state, kGameOver);

  gameOverInteraction(game, Left);
  ck_assert_int_eq(game->tetris->pause, kGameOverOne);

  gameOverInteraction(game, Start);
  ck_assert_int_eq(game->state, kSpawn);
  ck_assert_int_eq(game->tetris->pause, kNoPause);

  game->state = kGameOver;
  game->tetris->pause = kGameOverTwo;

  gameOverInteraction(game, Left);
  ck_assert_int_eq(game->tetris->pause, kGameOverTwo);

  gameOverInteraction(game, Start);
  ck_assert_int_eq(game->state, kExit);
}
END_TEST

Suite *menuInteractionTests() {
  Suite *s = suite_create("menuInteractionTest");
  TCase *tc_core = tcase_create("menuInteractionTest");

  tcase_add_test(tc_core, mainMenuInteractionTest);
  tcase_add_test(tc_core, pauseInteractionTest);
  tcase_add_test(tc_core, gameOverInteractionTest);

  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(createMatrixTest) {
  int code = kOk;
  int **matrix = createMatrix(TETR_SIZE, TETR_SIZE, &code);

  ck_assert_int_eq(code, 0);
  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      ck_assert_int_eq(matrix[i][j], 0);
    }
  }

  free(matrix);
}
END_TEST

START_TEST(turnRightMatrixTest) {
  int code = 0;
  int **matrix = createMatrix(TETR_SIZE, TETR_SIZE, &code);
  int counter = 1;
  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      matrix[i][j] = counter++;
    }
  }
  code = turnRightMatrix(TETR_SIZE, TETR_SIZE, matrix);
  ck_assert_int_eq(code, kOk);

  for (int j = 0; j < TETR_SIZE; j++) {
    for (int i = TETR_SIZE - 1; i >= 0; i--) {
      counter--;
      ck_assert_int_eq(matrix[i][j], counter);
    }
  }
  free(matrix);

  code = turnRightMatrix(3, 3, NULL);
  ck_assert_int_eq(code, kError);
}
END_TEST

START_TEST(turnLeftMatrixTest) {
  int code = 0;
  int **matrix = createMatrix(TETR_SIZE, TETR_SIZE, &code);
  int counter = 1;
  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      matrix[i][j] = counter++;
    }
  }
  code = turnLeftMatrix(TETR_SIZE, TETR_SIZE, matrix);
  ck_assert_int_eq(code, kOk);

  for (int j = TETR_SIZE - 1; j >= 0; j--) {
    for (int i = 0; i < TETR_SIZE; i++) {
      counter--;
      ck_assert_int_eq(matrix[i][j], counter);
    }
  }
  free(matrix);

  code = turnLeftMatrix(3, 3, NULL);
  ck_assert_int_eq(code, kError);
}
END_TEST

Suite *matrixTests() {
  Suite *s = suite_create("matrixTests");
  TCase *tc_core = tcase_create("matrixTests");

  tcase_add_test(tc_core, createMatrixTest);
  tcase_add_test(tc_core, turnRightMatrixTest);
  tcase_add_test(tc_core, turnLeftMatrixTest);

  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(checkingFieldRowsTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  for (int j = 0; j < FIELD_WIDTH; j++) {
    game->tetris->field[FIELD_HEIGHT - 1][j] = 1;
  }

  checkingFieldRows(game);
  ck_assert_int_eq(game->tetris->score, SCORE_ONE);

  prepareForNewGame();
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->tetris->field[FIELD_HEIGHT - 1 - i][j] = 1;
    }
  }
  checkingFieldRows(game);
  ck_assert_int_eq(game->tetris->score, SCORE_TWO);

  prepareForNewGame();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->tetris->field[FIELD_HEIGHT - 1 - i][j] = 1;
    }
  }
  checkingFieldRows(game);
  ck_assert_int_eq(game->tetris->score, SCORE_THREE);

  prepareForNewGame();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->tetris->field[FIELD_HEIGHT - 1 - i][j] = 1;
    }
  }
  checkingFieldRows(game);
  ck_assert_int_eq(game->tetris->score, SCORE_FOUR);
}
END_TEST

START_TEST(movingProcessingTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  spawningTetromino(game);

  movingProcessing(Terminate, false);
  ck_assert_int_eq(game->state, kExit);

  movingProcessing(Pause, false);
  ck_assert_int_eq(game->state, kPauseMenu);

  game->state = kMoving;
  game->tetris->pause = kNoPause;
  movingProcessing(Left, false);
  ck_assert_int_eq(game->state, kMoving);

  movingProcessing(Right, false);
  ck_assert_int_eq(game->state, kMoving);

  movingProcessing(Up, false);
  ck_assert_int_eq(game->state, kMoving);

  movingProcessing(Action, false);
  ck_assert_int_eq(game->state, kMoving);

  movingProcessing(Down, false);
  ck_assert_int_eq(game->state, kMoving);

  movingProcessing(Down, true);
  ck_assert_int_eq(game->state, kAttaching);
}
END_TEST

START_TEST(spawningTetrominoTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  spawningTetromino(game);
  ck_assert_int_eq(game->tetromino->y, 0);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH / 2);
  ck_assert_int_eq(game->state, kMoving);
  prepareForNewGame();

  for (int i = 0; i < 2; i++) {
    for (int j = 1; j < FIELD_WIDTH; j++) {
      game->tetris->field[i][j] = 1;
    }
  }

  spawningTetromino(game);
  ck_assert_int_eq(game->state, kGameOver);
  ck_assert_int_eq(game->tetris->pause, kGameOverOne);
}
END_TEST

START_TEST(movingDownTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  spawningTetromino(game);
  movingDown(game);
  ck_assert_int_eq(game->tetromino->y, 1);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH / 2);

  game->tetromino->y = FIELD_HEIGHT - 2;
  movingDown(game);
  ck_assert_int_eq(game->tetromino->y, FIELD_HEIGHT - 1);

  movingDown(game);
  ck_assert_int_eq(game->state, kAttaching);
}
END_TEST

START_TEST(movingLeftTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  spawningTetromino(game);
  movingLeft(game);
  ck_assert_int_eq(game->tetromino->y, 0);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH / 2 - 1);

  prepareForNewGame();
  settingTetromino(1, 2, game->tetromino->shape);
  game->tetromino->x = 2;
  game->tetromino->y = 0;
  game->tetromino->rotation_stage = 0;
  game->tetromino->index = 2;

  movingLeft(game);
  ck_assert_int_eq(game->tetromino->x, 1);

  movingLeft(game);
  ck_assert_int_eq(game->tetromino->x, 1);

  while (game->tetromino->rotation_stage != 3) rotatingTetromino(game);
  movingLeft(game);
  ck_assert_int_eq(game->tetromino->x, 0);
}
END_TEST

START_TEST(movingRightTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  spawningTetromino(game);
  movingRight(game);
  ck_assert_int_eq(game->tetromino->y, 0);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH / 2 + 1);

  prepareForNewGame();
  settingTetromino(1, 2, game->tetromino->shape);
  game->tetromino->x = FIELD_WIDTH - 4;
  game->tetromino->y = 0;
  game->tetromino->rotation_stage = 0;
  game->tetromino->index = 2;

  movingRight(game);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH - 3);

  movingRight(game);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH - 2);

  movingRight(game);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH - 2);

  rotatingTetromino(game);
  movingRight(game);
  ck_assert_int_eq(game->tetromino->x, FIELD_WIDTH - 1);
}
END_TEST

START_TEST(rotatingTetrominoTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  settingTetromino(1, 2, game->tetromino->shape);
  game->tetromino->rotation_stage = 0;
  game->tetromino->x = FIELD_WIDTH / 2;
  game->tetromino->y = 0;
  game->tetromino->index = 2;

  rotatingTetromino(game);
  ck_assert_int_eq(game->tetromino->rotation_stage, 1);

  rotatingTetromino(game);
  ck_assert_int_eq(game->tetromino->rotation_stage, 2);

  rotatingTetromino(game);
  ck_assert_int_eq(game->tetromino->rotation_stage, 3);

  rotatingTetromino(game);
  ck_assert_int_eq(game->tetromino->rotation_stage, 0);

  settingTetromino(1, 3, game->tetromino->shape);
  game->tetromino->rotation_stage = 0;
  game->tetromino->index = 3;

  rotatingTetromino(game);
  ck_assert_int_eq(game->tetromino->rotation_stage, 1);

  rotatingTetromino(game);
  ck_assert_int_eq(game->tetromino->rotation_stage, 0);
}
END_TEST

START_TEST(attachingTetrominoTest) {
  Singleton *game = *getState();

  prepareForNewGame();
  settingTetromino(1, 2, game->tetromino->shape);
  game->tetromino->rotation_stage = 0;
  game->tetromino->index = 2;
  game->tetromino->x = FIELD_WIDTH / 2;
  game->tetromino->y = 0;
  game->state = kMoving;
  movingDown(game);
  movingDown(game);
  attachingTetromino(game);
  ck_assert_int_eq(game->state, kSpawn);

  settingTetromino(1, 2, game->tetromino->shape);
  game->tetromino->rotation_stage = 0;
  game->tetromino->index = 2;
  game->tetromino->x = FIELD_WIDTH / 2;
  game->tetromino->y = 0;
  game->state = kMoving;

  movingDown(game);
  ck_assert_int_eq(game->state, kAttaching);

  attachingTetromino(game);
  ck_assert_int_eq(game->state, kGameOver);
}
END_TEST

Suite *fsmTests() {
  Suite *s = suite_create("fsmTests");
  TCase *tc_core = tcase_create("fsmTests");

  tcase_add_test(tc_core, checkingFieldRowsTest);
  tcase_add_test(tc_core, spawningTetrominoTest);
  tcase_add_test(tc_core, movingProcessingTest);
  tcase_add_test(tc_core, movingDownTest);
  tcase_add_test(tc_core, movingLeftTest);
  tcase_add_test(tc_core, movingRightTest);
  tcase_add_test(tc_core, rotatingTetrominoTest);
  tcase_add_test(tc_core, attachingTetrominoTest);

  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(timerTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  spawningTetromino(game);
  int counter_one = 0;
  int counter_two = 0;
  timer();
  while (!timer()) {
    counter_one++;
  }
  game->tetris->speed = 10;
  while (!timer()) {
    counter_two++;
  }

  ck_assert_int_eq(counter_one > counter_two, 1);
}
END_TEST

START_TEST(prepareForNewGameTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  ck_assert_int_eq(game->state, kSpawn);
  ck_assert_int_eq(game->tetris->level, 1);
  ck_assert_int_eq(game->tetris->speed, 1);
  ck_assert_int_eq(game->tetris->score, 0);
  ck_assert_int_eq(game->tetris->pause, 0);
}
END_TEST

START_TEST(updateCurrentStateTest) {
  Singleton *game = *getState();
  game->tetris->level = 10;
  game->tetris->score = 500;
  ck_assert_int_eq(updateCurrentState().level, 10);
  ck_assert_int_eq(updateCurrentState().score, 500);

  //   game->state = kExit;
  //   ck_assert_ptr_eq(updateCurrentState().field, NULL);
}
END_TEST

START_TEST(saveHighScoreTest) {
  Singleton *game = *getState();

  game->tetris->score = 1000;
  saveHighScore(game->tetris);
  ck_assert_int_eq(game->tetris->high_score, 1000);
  saveHighScore(game->tetris);
}
END_TEST

START_TEST(userInputTest) {
  Singleton *game = *getState();
  prepareForNewGame();
  userInput(None, false);
  ck_assert_int_eq(game->state, kMoving);

  userInput(Terminate, false);
  ck_assert_int_eq(game->state, kExit);

  game->state = kMainMenu;
  game->tetris->pause = kMainMenuOne;
  userInput(Start, false);
  ck_assert_int_eq(game->state, kSpawn);
  ck_assert_int_eq(game->tetris->pause, kNoPause);

  userInput(Up, false);
  ck_assert_int_eq(game->state, kMoving);

  int y = game->tetromino->y;
  while (y == game->tetromino->y) {
    userInput(Up, false);
  }
  ck_assert_int_eq(game->state, kMoving);

  userInput(Pause, false);
  ck_assert_int_eq(game->state, kPauseMenu);
  ck_assert_int_eq(game->tetris->pause, kPauseOne);

  userInput(Pause, false);
  ck_assert_int_eq(game->state, kMoving);

  userInput(Down, false);
  ck_assert_int_eq(game->state, kAttaching);

  userInput(Up, false);
  ck_assert_int_eq(game->state, kSpawn);

  userInput(Up, false);
  ck_assert_int_eq(game->state, kMoving);

  game->state = kGameOver;
  game->tetris->pause = kGameOverOne;
  userInput(Down, false);
  ck_assert_int_eq(game->tetris->pause, kGameOverTwo);

  game->state = kExit;
  userInput(None, false);

  ck_assert_ptr_eq(*getState(), NULL);
}
END_TEST

Suite *backendTests() {
  Suite *s = suite_create("backendTests");
  TCase *tc_core = tcase_create("backendTests");

  tcase_add_test(tc_core, timerTest);
  tcase_add_test(tc_core, prepareForNewGameTest);
  tcase_add_test(tc_core, updateCurrentStateTest);
  tcase_add_test(tc_core, saveHighScoreTest);
  tcase_add_test(tc_core, userInputTest);

  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int nf = 0;

  Suite *suites[TEST_BLOCKS] = {menuInteractionTests(), matrixTests(),
                                fsmTests(), backendTests()};

  for (int i = 0; i < TEST_BLOCKS; i++) {
    SRunner *sr = srunner_create(suites[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    nf += srunner_ntests_failed(sr);
    srunner_free(sr);
  }

  return nf == 0 ? 0 : 1;
}
