#include "tetris.h"

void userInput(UserAction_t action, bool hold) {
  if (action == Down) {
    hold = true;
  }

  Singleton *game = *getState();
  removeFigureFromField(game->tetris, game->tetromino);

  if (timer() && game->state == kMoving && action != Terminate) {
    action = Down;
    movingProcessing(action, hold);
  } else if (game->state == kMoving) {
    movingProcessing(action, hold);
  } else if (game->state == kSpawn) {
    spawningTetromino(game);
  } else if (game->state == kAttaching) {
    attachingTetromino(game);
  } else if (game->state == kMainMenu) {
    mainMenuInteraction(game, action);
  } else if (game->state == kPauseMenu) {
    pauseInteraction(game, action);
  } else if (game->state == kGameOver) {
    gameOverInteraction(game, action);
  } else if (game->state == kExit) {
    saveHighScore(game->tetris);
    removeGame();
  }

  if (*getState()) {
    mergeFieldWithFigure(game->tetris, game->tetromino);
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t tetris = {0};
  Singleton *game = *getState();

  if (game != NULL) {
    tetris = *(game->tetris);
    // int code = kOk;
    // int **temp = createMatrix(FIELD_HEIGHT, FIELD_WIDTH, &code);
    // if (code == kOk) {
    //   for (int i = 0; i < FIELD_HEIGHT; i++) {
    //     for (int j = 0; j < FIELD_WIDTH; j++) {
    //       temp[i][j] = tetris.field[i][j];
    //     }
    //   }
    //   tetris.field = temp;

    // Tetromino tetromino = *(game->tetromino);
    // mergeFieldWithFigure(&tetris, game->tetromino);
    // } else {
    //   removeGame();
    //   tetris.field = NULL;
    // }
  } else {
    tetris.field = NULL;
  }

  return tetris;
}

void removeFigureFromField(GameInfo_t *tetris, Tetromino *tetromino) {
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      if (tetris->field[i][j] == GHOST_CELL) {
        tetris->field[i][j] = 0;
      }
    }
  }

  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      if (tetromino->shape[i][j]) {
        int x_corrected = tetromino->x - 2 + j;
        int y_corrected = tetromino->y - 2 + i;
        if (y_corrected >= 0) {
          tetris->field[y_corrected][x_corrected] = 0;
        }
      }
    }
  }
}

void mergeFieldWithFigure(GameInfo_t *tetris, Tetromino *tetromino) {
  int ghost_shift = 0;
  int flag = 0;

  for (int k = 1; k < FIELD_HEIGHT && !flag; k++) {
    for (int i = 0; i < TETR_SIZE && !flag; i++) {
      for (int j = 0; j < TETR_SIZE && !flag; j++) {
        checkGhostPlace(tetris, tetromino, i, j, k, &flag);
      }
    }
    if (flag == 0) ghost_shift++;
  }

  for (int i = 0; i < TETR_SIZE; i++) {
    for (int j = 0; j < TETR_SIZE; j++) {
      if (tetromino->shape[i][j]) {
        int x_corrected = 0, y_corrected = 0;
        x_corrected = tetromino->x - 2 + j;
        y_corrected = tetromino->y - 2 + i;

        if (y_corrected + ghost_shift >= 0) {
          tetris->field[y_corrected + ghost_shift][x_corrected] = GHOST_CELL;
        }
        if (y_corrected >= 0) {
          tetris->field[y_corrected][x_corrected] = tetromino->shape[i][j];
        }
      }
    }
  }
}

void checkGhostPlace(GameInfo_t *tetris, Tetromino *tetromino, int i, int j,
                     int k, int *flag) {
  if (tetromino->shape[i][j] != 0) {
    int x_cor = tetromino->x - 2 + j;
    int y_cor = tetromino->y - 2 + i + k;
    if (y_cor >= FIELD_HEIGHT)
      *flag = 1;
    else if (y_cor >= 0 && tetris->field[y_cor][x_cor] != 0)
      *flag = 1;
  }
}

Singleton **getState() {
  static Singleton *game = NULL;
  static int game_started = 0;
  if (game == NULL && !game_started) {
    if (!game_started) {
      srand(time(NULL));
    }
    game_started = 1;
    game = malloc(sizeof(Singleton));
    if (game != NULL) {
      int code = createGameInfo(&game->tetris);
      if (code == kOk) {
        code = createTetromino(&game->tetromino);
        if (code == kError) {
          free(game->tetris->field);
          free(game->tetris->next);
          free(game->tetris);
        }
      }

      if (code == kError) {
        free(game);
        game = NULL;
      } else if (code == kOk)
        game->state = kMainMenu;
    }
  }

  return &game;
}

int createGameInfo(GameInfo_t **tetris) {
  int code = kOk;
  *tetris = malloc(sizeof(GameInfo_t));

  if (*tetris != NULL) {
    (*tetris)->field = createMatrix(FIELD_HEIGHT, FIELD_WIDTH, &code);
    if ((*tetris)->field != NULL) {
      (*tetris)->next = createMatrix(TETR_SIZE, TETR_SIZE, &code);
      if ((*tetris)->next == NULL) free((*tetris)->field);
    }
  } else
    code = kError;

  if (code == kOk) {
    (*tetris)->score = 0;
    getHighScore((*tetris));
    (*tetris)->level = 1;
    (*tetris)->speed = 1;
    (*tetris)->pause = kMainMenuOne;
  } else
    free(*tetris);

  return code;
}

int createTetromino(Tetromino **tetromino) {
  int code = 0;
  *tetromino = malloc(sizeof(Tetromino));
  if (*tetromino != NULL) {
    (*tetromino)->shape = createMatrix(TETR_SIZE, TETR_SIZE, &code);
  } else
    code = kError;

  if (code == kOk) {
    (*tetromino)->index = 0;
    (*tetromino)->rotation_stage = 0;
    (*tetromino)->x = 0;
    (*tetromino)->y = 0;
  } else
    free(*tetromino);

  return code;
}

void prepareForNewGame() {
  Singleton *game = *getState();
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->tetris->field[i][j] = 0;
    }
  }
  game->tetris->score = 0;
  getHighScore(game->tetris);
  game->tetris->level = 1;
  game->tetris->speed = 1;
  game->tetris->pause = kNoPause;
  game->state = kSpawn;
}

void removeGame() {
  Singleton **game = getState();
  if (*game != NULL) {
    if ((*game)->tetris != NULL) {
      free((*game)->tetris->field);
      free((*game)->tetris->next);
      free((*game)->tetris);
      (*game)->tetris = NULL;
    }

    if ((*game)->tetromino != NULL) {
      free((*game)->tetromino->shape);
      free((*game)->tetromino);
      (*game)->tetromino = NULL;
    }
    free(*game);
    *game = NULL;
  }
}

void getHighScore(GameInfo_t *tetris) {
  FILE *file = fopen(HIGH_SCORE_DIR, "r");
  if (file == NULL) {
    tetris->high_score = 0;
  } else {
    int temp = 0;
    fscanf(file, "%d", &temp);
    tetris->high_score = temp > 0 ? temp : 0;

    fclose(file);
  }
}

void saveHighScore(GameInfo_t *tetris) {
  if (tetris->score > tetris->high_score) {
    tetris->high_score = tetris->score;
    FILE *file = fopen(HIGH_SCORE_DIR, "w");
    if (file != NULL) {
      fprintf(file, "%d", tetris->high_score);
      fclose(file);
    }
  }
}

int timer() {
  static struct timespec start = {0, 0};
  static double time_failing = 0.0;
  static int initialized = 0;
  int alarm = 0;
  Singleton *game = *getState();
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);

  if (!initialized) {
    start = end;
    initialized = 1;
  } else if (game->state == kMoving) {
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (double)(end.tv_nsec - start.tv_nsec) / 1e9;

    time_failing += elapsed_time;
    double waiting_time = 1 - 0.08 * game->tetris->speed;
    if (time_failing > waiting_time) {
      alarm = 1;
      time_failing -= waiting_time;
    }
  } else if (game->state == kSpawn) {
    time_failing = 0.0;
  }
  start = end;

  return alarm;
}