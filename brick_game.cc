#include "gui/desktop/desktop.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gui/cli/cli.h"

#ifdef __cplusplus
}
#endif

#include "headers/brick_game.h"

int main() {
  int interface = 0, game = 0;

  std::cout << "Choose the interface:" << std::endl;
  std::cout << "1 - Terminal" << std::endl;
  std::cout << "2 - Window" << std::endl;
  std::cin >> interface;

  std::cout << "Choose the game:" << std::endl;
  std::cout << "1 - Tetris" << std::endl;
  std::cout << "2 - Snake" << std::endl;
  std::cin >> game;

  if (interface == 1 && game == 1) {
    ConsoleInterface(updateCurrentState, userInput);
  } else if (interface == 1 && game == 2) {
    ConsoleInterface(s21::SnakeController::updateCurrentState,
                     s21::SnakeController::userInput);
  } else if (interface == 2 && game == 1) {
    DesktopInterface(updateCurrentState, userInput);
  } else if (interface == 2 && game == 2) {
    DesktopInterface(s21::SnakeController::updateCurrentState,
                     s21::SnakeController::userInput);
  }

  return 0;
}

void ConsoleInterface(GameInfo_t (*updateCurrentState)(),
                      void (*userInput)(UserAction_t, bool)) {
  createWindow();

  GameInfo_t game = updateCurrentState();
  while (game.field) {
    game = updateCurrentState();
    if (game.field) {
      gameDisplay(&game);
      userInput(inputProcessing(), false);
    }
  }

  endwin();
}

int DesktopInterface(GameInfo_t (*updateCurrentState)(),
                     void (*userInput)(UserAction_t, bool)) {
  int argc = 0;
  char **argv = nullptr;
  QApplication app(argc, argv);

  s21::GameView window(userInput, updateCurrentState);
  window.setWindowTitle("BrickGameV2.0");
  window.show();

  return app.exec();
}
