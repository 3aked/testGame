#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "errors.h"
#include "ui_mainwindow.h"
#include <QMainWindow>

class Client;
class Game;
class ClientDefWidget;
class InGameWidget;
class Finish;

class MainWindow : public QMainWindow, public Ui::MainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void startGame(bool autoplay);
  void showResultat();

private:
  void initClient();
  void connectGame(std::shared_ptr<Game> game);
  void handleClientError(const ErrorData &error);

  ClientDefWidget *const m_clientDefWidget;
  InGameWidget *const m_inGameWidget;
  Finish *const m_finish;

  std::shared_ptr<Client> m_client;
  std::shared_ptr<Game> m_currentGame;
};

#endif // MAINWINDOW_H
