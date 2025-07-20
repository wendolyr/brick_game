#include "tetris.h"

void settingTetromino(int color, int type, int **shape) {
  int tetromino_types[7][TETR_SIZE][TETR_SIZE] = {
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {color, color, color, color, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0},
       {0, color, 0, 0, 0},
       {0, color, color, color, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0},
       {0, color, color, 0, 0},
       {0, 0, color, color, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0},
       {0, color, color, 0, 0},
       {0, color, color, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0},
       {0, 0, color, color, 0},
       {0, color, color, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0},
       {0, 0, color, 0, 0},
       {0, color, color, color, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0},
       {0, 0, 0, color, 0},
       {0, color, color, color, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}}};

  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      shape[i][j] = tetromino_types[type - 1][i][j];
    }
  }
}

void spawningTetromino(Singleton *game) {
  static int new_game = 1;
  static int prev_form = 0;
  if (!new_game) {
    for (int i = 0; i < TETR_SIZE; i++) {
      for (int j = 0; j < TETR_SIZE; j++) {
        game->tetromino->shape[i][j] = game->tetris->next[i][j];
      }
    }
    game->tetromino->index = prev_form;
  }

#ifdef __APPLE__
  for (int i = 0; i < 10; i++) {
    rand();
  }
#endif

  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      game->tetris->next[i][j] = 0;
    }
  }

  int form = 1 + rand() % 7;
  prev_form = form;
  int color = 1 + rand() % 7;
  settingTetromino(color, form, game->tetris->next);

  if (new_game) {
    new_game = 0;
    spawningTetromino(game);
  }

  game->tetromino->x = FIELD_WIDTH / 2;
  game->tetromino->y = 0;
  game->tetromino->rotation_stage = 0;

  if (collisionCheck(game)) {
    game->state = kGameOver;
    game->tetris->pause = kGameOverOne;
    new_game = 1;
  } else
    game->state = kMoving;
}

void movingProcessing(UserAction_t action, bool hold) {
  Singleton *game = *getState();

  if (action == Terminate) {
    saveHighScore(game->tetris);
    game->state = kExit;
  } else if (action == Pause) {
    game->state = kPauseMenu;
    game->tetris->pause = kPauseOne;
  } else if (action == Left) {
    movingLeft(game);
  } else if (action == Right) {
    movingRight(game);
  } else if (action == Down && hold == false) {
    movingDown(game);
  } else if (action == Down && hold == true) {
    int code = 1;
    while (code == 1) {
      code = movingDown(game);
    }
  } else if (action == Action) {
    rotatingTetromino(game);
  }
}

int movingDown(Singleton *game) {
  int can_move = 1;
  game->tetromino->y += 1;

  if (collisionCheck(game)) {
    game->tetromino->y -= 1;
    game->state = kAttaching;
    can_move = 0;
  }

  return can_move;
}

void movingLeft(Singleton *game) {
  game->tetromino->x -= 1;

  if (collisionCheck(game)) {
    game->tetromino->x += 1;
  }
}

void movingRight(Singleton *game) {
  game->tetromino->x += 1;

  if (collisionCheck(game)) {
    game->tetromino->x -= 1;
  }
}

void rotatingTetromino(Singleton *game) {
  if (game->tetromino->index == 2 || game->tetromino->index == 6 ||
      game->tetromino->index == 7) {
    turnLeftMatrix(TETR_SIZE, TETR_SIZE, game->tetromino->shape);

    if (collisionCheck(game))
      turnRightMatrix(TETR_SIZE, TETR_SIZE, game->tetromino->shape);
    else
      game->tetromino->rotation_stage =
          (game->tetromino->rotation_stage + 1) % 4;

  } else if (game->tetromino->index == 1 || game->tetromino->index == 3 ||
             game->tetromino->index == 5) {
    if (game->tetromino->rotation_stage == 0) {
      turnLeftMatrix(TETR_SIZE, TETR_SIZE, game->tetromino->shape);

      if (collisionCheck(game)) {
        turnRightMatrix(TETR_SIZE, TETR_SIZE, game->tetromino->shape);
      } else {
        game->tetromino->rotation_stage =
            (game->tetromino->rotation_stage + 1) % 2;
      }
    } else {
      turnRightMatrix(TETR_SIZE, TETR_SIZE, game->tetromino->shape);

      if (collisionCheck(game)) {
        turnLeftMatrix(TETR_SIZE, TETR_SIZE, game->tetromino->shape);
      } else {
        game->tetromino->rotation_stage =
            (game->tetromino->rotation_stage + 1) % 2;
      }
    }
  }
}

int collisionCheck(Singleton *game) {
  int collision = 0;

  for (int i = 0; i < TETR_SIZE && !collision; i++) {
    for (int j = 0; j < TETR_SIZE && !collision; j++) {
      if (game->tetromino->shape[i][j] != 0) {
        int x_curr = game->tetromino->x - 2 + j;
        int y_curr = game->tetromino->y - 2 + i;

        if (x_curr < 0 || x_curr >= FIELD_WIDTH || y_curr >= FIELD_HEIGHT) {
          collision = 1;
        } else if (y_curr >= 0) {
          collision = game->tetris->field[y_curr][x_curr] != 0 ? 1 : 0;
        }
      }
    }
  }

  return collision;
}

int attachingTetromino(Singleton *game) {
  int permission = 1;
  for (int i = 0; i < TETR_SIZE && permission; i++) {
    for (int j = 0; j < TETR_SIZE && permission; j++) {
      if (game->tetromino->shape[i][j] != 0) {
        int x_corrected = game->tetromino->x - 2 + j;
        int y_corrected = game->tetromino->y - 2 + i;
        if (y_corrected >= 0)
          game->tetris->field[y_corrected][x_corrected] =
              game->tetromino->shape[i][j];
        else
          permission = 0;
      }
    }
  }

  for (int i = 0; i < TETR_SIZE; ++i) {
    for (int j = 0; j < TETR_SIZE; ++j) {
      game->tetromino->shape[i][j] = 0;
    }
  }

  if (permission) {
    checkingFieldRows(game);
    game->state = kSpawn;
  } else {
    game->state = kGameOver;
    game->tetris->pause = kGameOverOne;
  }

  return permission;
}

void checkingFieldRows(Singleton *game) {
  int destroyed_rows = 0;
  for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
    int counter = 0;
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (game->tetris->field[i][j] != 0) {
        counter++;
      }
    }
    if (counter == FIELD_WIDTH) {
      destroyRow(game->tetris, i);
      i++;
      destroyed_rows++;
    }
  }

  if (destroyed_rows != 0) {
    scoring(game, destroyed_rows);
  }
}

void destroyRow(GameInfo_t *tetris, int row) {
  for (int i = row; i >= 0; i--) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (i != 0)
        tetris->field[i][j] = tetris->field[i - 1][j];
      else
        tetris->field[i][j] = 0;
    }
  }
}

void scoring(Singleton *game, int destroyed_rows) {
  if (destroyed_rows == 1) {
    game->tetris->score += SCORE_ONE;
  } else if (destroyed_rows == 2) {
    game->tetris->score += SCORE_TWO;
  } else if (destroyed_rows == 3) {
    game->tetris->score += SCORE_THREE;
  } else if (destroyed_rows == 4) {
    game->tetris->score += SCORE_FOUR;
  }

  if (game->tetris->level < MAX_LEVEL) {
    game->tetris->level = 1 + game->tetris->score / LEVELUP_SCORE;
    if (game->tetris->level > MAX_LEVEL) game->tetris->level = MAX_LEVEL;
    game->tetris->speed = game->tetris->level;
  }
}