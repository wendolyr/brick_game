#include "desktop.h"

namespace s21 {

GameView::GameView(void (*userInput)(UserAction_t, bool),
                   GameInfo_t (*updateCurrentState)(), QWidget *parent)
    : QWidget(parent),
      userInput(userInput),
      updateCurrentState(updateCurrentState),
      game_{},
      text_color_(Qt::red) {
  setFixedSize(500, 600);
  setFocus();
  CreateButtons();

  connect(&game_timer_, &QTimer::timeout, this, &GameView::GameLoop);
  game_timer_.start(1);

  QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);

  QPropertyAnimation *toRed = new QPropertyAnimation(this, "textColor", group);
  toRed->setStartValue(QColor(255, 255, 0));
  toRed->setEndValue(QColor(255, 0, 0));
  toRed->setDuration(1000);

  QPropertyAnimation *toYellow =
      new QPropertyAnimation(this, "textColor", group);
  toYellow->setStartValue(QColor(255, 0, 0));
  toYellow->setEndValue(QColor(255, 255, 0));
  toYellow->setDuration(1000);

  group->addAnimation(toRed);
  group->addAnimation(toYellow);
  group->setLoopCount(-1);
  group->start();
}

GameView::~GameView() {}

void GameView::CreateButtons() {
  button_group_ = new QButtonGroup(this);

  RoundButton *up_button = new RoundButton("↑", this);
  RoundButton *down_button = new RoundButton("↓", this);
  RoundButton *left_button = new RoundButton("←", this);
  RoundButton *right_button = new RoundButton("→", this);
  RoundButton *action_button = new RoundButton("Action", this);
  RoundButton *pause_button = new RoundButton("Pause", this);
  RoundButton *enter_button = new RoundButton("Enter", this);

  button_group_->addButton(up_button, Up);
  button_group_->addButton(down_button, Down);
  button_group_->addButton(left_button, Left);
  button_group_->addButton(right_button, Right);
  button_group_->addButton(action_button, Action);
  button_group_->addButton(pause_button, Pause);
  button_group_->addButton(enter_button, Start);

  up_button->move(425, 400);
  down_button->move(425, 450);
  left_button->move(400, 425);
  right_button->move(450, 425);
  action_button->move(330, 413);
  pause_button->move(330, 485);
  enter_button->move(412, 485);

  connect(button_group_, &QButtonGroup::idClicked, this,
          &GameView::HandleButtonClick);
}

void GameView::HandleButtonClick(int action) {
  userInput(static_cast<UserAction_t>(action), false);
  this->setFocus();
}

void GameView::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  DrawStats(painter);

  if (game_.pause == static_cast<int>(PauseT::kNoPause)) {
    DrawField(painter);
  } else if (game_.pause == static_cast<int>(PauseT::kMenuOne) ||
             game_.pause == static_cast<int>(PauseT::kMenuTwo)) {
    DrawMenu(painter);
  } else if (game_.pause == static_cast<int>(PauseT::kGameOverOne) ||
             game_.pause == static_cast<int>(PauseT::kGameOverTwo)) {
    DrawGameOver(painter);
  } else {
    DrawPause(painter);
  }
}

void GameView::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = None;
  bool hold = false;

  switch (event->key()) {
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    case Qt::Key_Escape:
      action = Terminate;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
      action = Start;
      break;
  }

  userInput(action, hold);
}

void GameView::GameLoop() {
  game_ = updateCurrentState();

  if (!game_.field) {
    game_timer_.stop();
    close();
    return;
  }

  userInput(None, false);
  update();
}

void GameView::DrawStats(QPainter &painter) {
  QFont font("Arial", 14);
  font.setBold(true);
  painter.setFont(font);

  painter.setPen(Qt::black);

  painter.drawText(310, 30, QString("Record:"));
  painter.drawText(310, 80, QString("Score:"));
  painter.drawText(310, 130, QString("Level:"));
  painter.drawText(310, 180, QString("Next:"));

  font.setBold(false);
  painter.setFont(font);
  painter.drawText(400, 50, QString("%1").arg(game_.high_score));
  painter.drawText(400, 100, QString("%1").arg(game_.score));
  painter.drawText(400, 150, QString("%1").arg(game_.level));
}

void GameView::DrawField(QPainter &painter) {
  if (!game_.field) {
    return;
  }

  int cell_width = 300 / kWidth;
  int cell_height = 600 / kHeight;

  for (int y = 0; y < kHeight; ++y) {
    for (int x = 0; x < kWidth; ++x) {
      QRect rect(x * cell_width, y * cell_height, cell_width, cell_height);
      DrawCell(game_.field[y][x], rect, painter);
    }
  }

  for (int y = 0; y < kNext && game_.next; ++y) {
    for (int x = 0; x < kNext; ++x) {
      QRect rect(330 + x * cell_width, 200 + y * cell_height, cell_width,
                 cell_height);
      DrawCell(game_.next[y][x], rect, painter);
    }
  }
}

void GameView::DrawCell(int cell_color, QRect &rect, QPainter &painter) {
  if (cell_color == 1) {
    painter.fillRect(rect, Qt::red);
    painter.setPen(Qt::darkRed);
  } else if (cell_color == 2) {
    painter.fillRect(rect, Qt::green);
    painter.setPen(Qt::darkGreen);
  } else if (cell_color == 3) {
    painter.fillRect(rect, Qt::yellow);
    painter.setPen(Qt::darkYellow);
  } else if (cell_color == 4) {
    painter.fillRect(rect, Qt::blue);
    painter.setPen(Qt::darkBlue);
  } else if (cell_color == 5) {
    painter.fillRect(rect, Qt::magenta);
    painter.setPen(Qt::darkMagenta);
  } else if (cell_color == 6) {
    painter.fillRect(rect, Qt::cyan);
    painter.setPen(Qt::darkCyan);
  } else if (cell_color == 7) {
    painter.fillRect(rect, Qt::white);
    painter.setPen(Qt::darkGray);
  } else {
    painter.fillRect(rect, Qt::gray);
    painter.setPen(Qt::darkGray);
  }
  painter.drawRect(rect);
}

void GameView::DrawMenu(QPainter &painter) {
  painter.fillRect(rect().adjusted(0, 0, -200, 0), Qt::black);

  painter.setPen(text_color_);

  QFont font("Arial", 20);
  font.setBold(true);
  painter.setFont(font);

  painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 100)),
                   Qt::AlignCenter, "BRICK GAME");

  font.setPointSize(12);
  painter.setFont(font);

  if (game_.next) {
    painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 50)),
                     Qt::AlignCenter, "TETRIS");
  } else {
    painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 50)),
                     Qt::AlignCenter, "SNAKE");
  }

  font.setFamily("Impact");
  font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
  painter.setFont(font);

  painter.setPen(game_.pause == static_cast<int>(PauseT::kMenuOne) ? Qt::red
                                                                   : Qt::white);
  painter.drawText(rect().adjusted(0, 0, -200, -50), Qt::AlignCenter, "START");

  painter.setPen(game_.pause == static_cast<int>(PauseT::kMenuTwo) ? Qt::red
                                                                   : Qt::white);
  painter.drawText(rect().adjusted(0, 0, -200, 50), Qt::AlignCenter, "EXIT");
}

void GameView::DrawPause(QPainter &painter) {
  painter.fillRect(rect().adjusted(0, 0, -200, 0), Qt::black);

  painter.setPen(text_color_);
  QFont font("Arial", 20);
  font.setBold(true);
  painter.setFont(font);

  painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 100)),
                   Qt::AlignCenter, "PAUSE");

  font.setPointSize(12);
  font.setFamily("Impact");
  font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
  painter.setFont(font);

  painter.setPen(game_.pause == static_cast<int>(PauseT::kPauseOne)
                     ? Qt::yellow
                     : Qt::white);
  painter.drawText(rect().adjusted(0, 0, -200, -50), Qt::AlignCenter, "RESUME");

  painter.setPen(game_.pause == static_cast<int>(PauseT::kPauseTwo)
                     ? Qt::yellow
                     : Qt::white);
  painter.drawText(rect().adjusted(0, 0, -200, 50), Qt::AlignCenter, "EXIT");
}

void GameView::DrawGameOver(QPainter &painter) {
  painter.fillRect(rect().adjusted(0, 0, -200, 0), Qt::black);

  painter.setPen(text_color_);
  QFont font("Arial", 20);
  font.setBold(true);
  painter.setFont(font);

  if (game_.level == kMaxLevel + 1) {
    painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 100)),
                     Qt::AlignCenter, "YOU WON!");
  } else {
    painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 100)),
                     Qt::AlignCenter, "GAME OVER");
  }
  font.setPointSize(12);
  painter.setFont(font);

  if (game_.score > game_.high_score) {
    painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2 + 50)),
                     Qt::AlignCenter, "Congrats!");
    painter.drawText(rect().adjusted(0, 0, -200, -(height() / 2)),
                     Qt::AlignCenter, "You beat the record!");
  }

  font.setPointSize(12);
  font.setFamily("Impact");
  font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
  painter.setFont(font);

  painter.setPen(game_.pause == static_cast<int>(PauseT::kGameOverOne)
                     ? Qt::yellow
                     : Qt::white);
  painter.drawText(rect().adjusted(0, 0, -200, -50), Qt::AlignCenter,
                   "RESTART");

  painter.setPen(game_.pause == static_cast<int>(PauseT::kGameOverTwo)
                     ? Qt::yellow
                     : Qt::white);
  painter.drawText(rect().adjusted(0, 0, -200, 50), Qt::AlignCenter, "EXIT");
}

GameView::RoundButton::RoundButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
  if (text == "↑" || text == "↓" || text == "←" || text == "→") {
    setFixedSize(25, 25);
  } else {
    setFixedSize(50, 50);
  }
}

void GameView::RoundButton::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QPainterPath path;
  path.addEllipse(rect());
  painter.fillPath(path, Qt::lightGray);

  painter.setPen(Qt::black);
  painter.drawText(rect(), Qt::AlignCenter, text());
}

QColor GameView::GetTextColor() const { return text_color_; }

void GameView::SetTextColor(const QColor &color) {
  text_color_ = color;
  update();
}

}  // namespace s21