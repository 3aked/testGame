#include "ingamewidget.h"
#include "game.h"

InGameWidget::InGameWidget(QWidget *parent)
    : QWidget(parent), Ui::InGameWidget() {
  setupUi(this);

  connect(pushButton_send, &QPushButton::clicked, this, [this] {
    if (!m_game) {
      return;
    }
    m_game->play(spinBox_number->value());
  });
}

InGameWidget::~InGameWidget() {}

void InGameWidget::setCurrentGame(std::shared_ptr<Game> game) {
  if (m_game) {
    disconnect(m_game.get(), 0, this, 0);
    if (m_game->state() == EGameState::InProgress) {
      m_game->abandon();
    }
  }
  m_game = game;
  auto bounds = m_game->bounds();

  spinBox_number->setMinimum(bounds.first);
  spinBox_number->setMaximum(bounds.second);

  QStringList infosServer;
  infosServer
      << QString("[min, max] = [%1, %2]").arg(bounds.first).arg(bounds.second)
      << QString("Max. number of tries: %1")
             .arg(m_game->triesMax() == -1
                      ? "Unlimited"
                      : QString::number(m_game->triesMax()));

  label_infosServer->setText(infosServer.join("\n"));

  connect(m_game.get(), &Game::updated, this, &InGameWidget::updateGame);
  connect(m_game.get(), &Game::stateChanged, this, &InGameWidget::updateGame);

  label_compare->clear();
  label_triesLeft->clear();
  label_state->setVisible(false);

  frame_game->setEnabled(true);
}

void InGameWidget::updateGame() {

  spinBox_number->setValue(m_game->lastNumber());
  label_compare->setText(toString(m_game->lastCompare()));

  label_triesLeft->setText(QString("%1/%2").arg(
      QString::number(m_game->tries()),
      (m_game->triesMax() == -1) ? "Unlimited"
                                 : QString::number(m_game->triesMax())));

  frame_game->setEnabled(m_game->state() == EGameState::InProgress);

  if (m_game->state() == EGameState::InProgress) {
    label_state->setVisible(false);

  } else {
    label_state->setVisible(true);

    label_state->setText(toString(m_game->state()));
  }
}
