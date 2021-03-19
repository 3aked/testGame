#include "game.h"

#include <QWebSocket>

#include <QRandomGenerator>

Game::Game(QWebSocket *socket, bool autoplay, int triesMax, bounds_t bounds,
           QObject *parent)
    : QObject(parent), m_socket{socket}, m_autoplay{autoplay},
      m_triesMax{triesMax}, m_bounds{bounds}, m_tries{0},
      m_state{EGameState::uninitialized}, m_startDate{
                                              QDateTime::currentDateTime()} {

  connect(m_socket, &QWebSocket::textMessageReceived, this,
          &Game::handleNewMessage);
  connect(m_socket, qOverload<QAbstractSocket::SocketError>(&QWebSocket::error),
          this, [this] {
            handleErrorData(
                ErrorData(EError::NetworkError, m_socket->errorString()));
          });

  if (m_autoplay) {
    m_autoBounds = m_bounds;
    autoPlay();
  }
}

Game::~Game() { m_socket->deleteLater(); }

void Game::setState(EGameState state) {
  if (m_state != state) {
    m_state = state;
    if (m_state != EGameState::InProgress) {
      m_socket->close();
    }
    emit stateChanged(m_state);
  }
}

void Game::handleNewMessage(const QString &message) {
  qDebug().noquote() << message;

  try {
    auto d = Data::parse(message);
    if (d->type() == Data::Error) {
      auto errorData = std::static_pointer_cast<ErrorData>(d);
      handleErrorData(*errorData.get());
    } else if (d->type() == Data::GuessReply) {
      auto guessReplyData = std::static_pointer_cast<GuessReplyData>(d);
      handleGuessReplyData(*guessReplyData.get());
    } else {
      // nothing to do
    }
  } catch (const ErrorData &error) {
    emit errorOccured(error);
  }
}

void Game::handleErrorData(const ErrorData &errorData) {
  qDebug() << "Data::Error" << errorData.toJson();
  emit errorOccured(errorData);
}

void Game::handleGuessReplyData(const GuessReplyData &guessReplyData) {

  m_lastCompare = guessReplyData.m_compare;
  m_tries = guessReplyData.m_tries;
  m_servernumber = guessReplyData.m_number;
  setState(guessReplyData.m_gameState);
  emit updated();

  if (m_autoplay && m_state == EGameState::InProgress) {
    if (m_lastCompare == ECompare::Bigger) {
      m_autoBounds.second = m_lastGuessData.m_number;
    } else {
      m_autoBounds.first = m_lastGuessData.m_number;
    }
    autoPlay();
  }
}

void Game::autoPlay() {

  auto number = QRandomGenerator::global()->bounded(m_autoBounds.first,
                                                    m_autoBounds.second);

  play(number);
}

void Game::play(int number) {
  m_lastGuessData.m_number = number;
  m_socket->sendTextMessage(m_lastGuessData.toJsonString());
}

void Game::abandon()
{
    setState(EGameState::Abondoned);
}


