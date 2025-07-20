#include "tetris.h"

void mainMenuInteraction(Singleton *game, UserAction_t action) {
  if (action == Terminate) {
    game->state = kExit;
  } else if (game->tetris->pause == kMainMenuOne) {
    if (action == Down)
      game->tetris->pause = kMainMenuTwo;
    else if (action == Start) {
      game->state = kSpawn;
      game->tetris->pause = kNoPause;
    }
  } else if (game->tetris->pause == kMainMenuTwo) {
    if (action == Up)
      game->tetris->pause = kMainMenuOne;
    else if (action == Start)
      game->state = kExit;
  }
}

void gameOverInteraction(Singleton *game, UserAction_t action) {
  if (action == Terminate) {
    game->state = kExit;
  } else if (game->tetris->pause == kGameOverOne) {
    if (action == Start) {
      saveHighScore(game->tetris);
      prepareForNewGame();
    } else if (action == Down)
      game->tetris->pause = kGameOverTwo;

  } else if (game->tetris->pause == kGameOverTwo) {
    if (action == Start) {
      game->state = kExit;
    } else if (action == Up)
      game->tetris->pause = 5;
  }
}

void pauseInteraction(Singleton *game, UserAction_t action) {
  if (action == Terminate) {
    game->state = kExit;
  } else if (action == Pause) {
    game->state = kMoving;
    game->tetris->pause = kNoPause;
  } else if (game->tetris->pause == kPauseOne) {
    if (action == Start) {
      game->state = kMoving;
      game->tetris->pause = kNoPause;
    } else if (action == Down)
      game->tetris->pause = kPauseTwo;
  } else if (game->tetris->pause == kPauseTwo) {
    if (action == Start) {
      game->state = kExit;
    } else if (action == Up)
      game->tetris->pause = kPauseOne;
  }
}