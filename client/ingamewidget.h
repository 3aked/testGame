#ifndef INGAMEWIDGET_H
#define INGAMEWIDGET_H

#include "ui_ingamewidget.h"
#include <QWidget>

class Game;

class InGameWidget : public QWidget, public Ui::InGameWidget {
  Q_OBJECT

public:
  explicit InGameWidget(QWidget *parent = nullptr);
  ~InGameWidget();

  void setCurrentGame(std::shared_ptr<Game> game);

private:
  void updateGame();

  std::shared_ptr<Game> m_game;
};

#endif // INGAMEWIDGET_H
