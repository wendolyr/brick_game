/**
 * @file desktop.h
 * @brief Header file for all desktop frontend functions
 */
#ifndef BRICKGAME_GUI_DESKTOP_DESKTOP_H
#define BRICKGAME_GUI_DESKTOP_DESKTOP_H

#include <QApplication>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QWidget>

#include "../../headers/snake_defines.h"
#include "../../headers/specification.h"

namespace s21 {

/**
 * @brief Main game view class implementing GUI for BrickGame
 */
class GameView : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QColor textColor READ GetTextColor WRITE SetTextColor)

 private:
  /**
   * @brief Custom round button implementation
   */
  class RoundButton : public QPushButton {
   public:
    /**
     * @brief Constructs a round button
     * @param text - button text
     * @param parent - parent widget
     */
    explicit RoundButton(const QString &text, QWidget *parent = nullptr);

   protected:
    /**
     * @brief Custom paint event for round button
     * @param event - paint event
     */
    void paintEvent(QPaintEvent *) override;
  };

 public:
  /**
   * @brief Constructs the game view
   * @param userInput - callback for user input processing
   * @param updateCurrentState - callback for getting current game state
   * @param parent - parent widget
   */
  explicit GameView(void (*userInput)(UserAction_t, bool),
                    GameInfo_t (*updateCurrentState)(),
                    QWidget *parent = nullptr);

  /**
   * @brief Destructor
   */
  ~GameView();

 protected:
  /**
   * @brief Main paint event handler
   * @param event - paint event
   */
  void paintEvent(QPaintEvent *event) override;

  /**
   * @brief Keyboard input handler
   * @param event - key event
   */
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  /**
   * @brief Main game loop handler
   */
  void GameLoop();

  /**
   * @brief Button click handler
   * @param action - button action ID
   */
  void HandleButtonClick(int action);

 private:
  /**
   * @brief Creates and positions all game buttons
   */
  void CreateButtons();

  /**
   * @brief Draws game statistics (score, level etc.)
   * @param painter - QPainter object
   */
  void DrawStats(QPainter &painter);

  /**
   * @brief Draws main game field
   * @param painter - QPainter object
   */
  void DrawField(QPainter &painter);

  /**
   * @brief Draws a single cell on the field
   * @param cell_color - color index of the cell
   * @param rect - cell rectangle
   * @param painter - QPainter object
   */
  void DrawCell(int cell_color, QRect &rect, QPainter &painter);

  /**
   * @brief Draws main menu screen
   * @param painter - QPainter object
   */
  void DrawMenu(QPainter &painter);

  /**
   * @brief Draws game over screen
   * @param painter - QPainter object
   */
  void DrawGameOver(QPainter &painter);

  /**
   * @brief Draws pause menu screen
   * @param painter - QPainter object
   */
  void DrawPause(QPainter &painter);

  /**
   * @brief Gets current text color for animations
   * @return current text color
   */
  QColor GetTextColor() const;

  /**
   * @brief Sets text color for animations
   * @param color - new text color
   */
  void SetTextColor(const QColor &color);

  GameInfo_t game_;    ///< Current game state information
  QTimer game_timer_;  ///< Timer for refreshing the game screen
  void (*userInput)(UserAction_t, bool);  ///< User input callback
  GameInfo_t (*updateCurrentState)();     ///< Game state update callback

  QColor text_color_;           ///< Current text color for animations
  QButtonGroup *button_group_;  ///< Group of control buttons
};

}  // namespace s21

#endif