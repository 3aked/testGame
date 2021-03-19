#include "mainwindow.h"

#include "client.h"
#include "clientdefwidget.h"
#include "ingamewidget.h"
#include"finish.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(),
      m_clientDefWidget{new ClientDefWidget(this)},
      m_inGameWidget{new InGameWidget(this)},
      m_finish{new Finish(this)}, m_client{nullptr} {
  setupUi(this);

  qApp->setStyleSheet("QLabel {police - taille: 50px }");


  stackedWidget->addWidget(m_clientDefWidget);
  stackedWidget->addWidget(m_inGameWidget);
  stackedWidget->addWidget(m_finish);

  // connections
  connect(
      m_clientDefWidget->pushButton_connect, &QPushButton::clicked, this,
      [this] { startGame(m_clientDefWidget->checkBox_autoPlay->isChecked()); });

  connect(
      m_inGameWidget->pushButton_newGame, &QPushButton::clicked, this,
      [this] { startGame(m_inGameWidget->checkBox_autoPlay->isChecked()); });

  connect(
      m_inGameWidget->pushButton_finish, &QPushButton::clicked, this,
      [this] { showResultat(); });
}

MainWindow::~MainWindow() {}

void MainWindow::startGame(bool autoplay) {
  if (!m_client) {
    initClient();
  }
  m_client->createGame(autoplay);
}

void MainWindow::showResultat()
{
    stackedWidget->setCurrentWidget(m_finish);
}

void MainWindow::initClient() {
  m_client = m_clientDefWidget->getClient();
  connect(m_client.get(), &Client::errorOccured, this,
          &MainWindow::handleClientError);
  connect(m_client.get(), &Client::gameCreated, this, &MainWindow::connectGame);
}

void MainWindow::connectGame(std::shared_ptr<Game> game) {

  stackedWidget->setCurrentWidget(m_inGameWidget);
  m_inGameWidget->setCurrentGame(game);
}

void MainWindow::handleClientError(const ErrorData &error) {
  m_clientDefWidget->errorLabel->setText(error.errorString());
}
