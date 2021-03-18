#include "game.h"

#include "errors.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QWebSocket>

#include <memory>

Game::Game(QWebSocket* const socket, int triesMax, bounds_t bounds,
           QObject* parent)
    : QObject(parent),
      m_socket{socket},
      m_number{
          QRandomGenerator::global()->bounded(bounds.first, bounds.second)},
      m_triesMax{triesMax},
      m_bounds{bounds},
      m_tries{0},
      m_autoplay{false},
      m_state{EGameState::uninitialized} {

    connect(m_socket, &QWebSocket::aboutToClose, this, &Game::aboutToClose);
    connect(m_socket, &QWebSocket::textMessageReceived, this,
            &Game::handleNewMessage);
}

int Game::remainingTries() const { return m_triesMax - m_tries; }

void Game::handleNewMessage(const QString& msg) {
    try {

        qDebug().noquote() << msg;
        if (m_state == EGameState::uninitialized) {
            // expect initialization message
            handleInitialization(msg);
        } else if (m_state == EGameState::InProgress) {
            // expect guess message
            handleGuess(msg);
        } else {
            // nothing to do
        }
    } catch (const Error& error) {
        m_socket->sendTextMessage(error.toJsonString());
    }
}

void Game::handleInitialization(const QString& msg) {
    auto data = Data::parse<InitializationData>(msg);

    m_name = data->m_name;
    m_startDate = QDateTime::currentDateTimeUtc();
    m_autoplay = data->m_autoplay;

    InitializationReplyData reply;
    reply.m_maxTries = m_triesMax;
    reply.m_bounds = m_bounds;

    if (m_autoplay) {
        setState(EGameState::AutoPlay);
        m_autoBounds = m_bounds;
    } else {
        setState(EGameState::InProgress);
    }
    reply.m_gameState = m_state;
    m_socket->sendTextMessage(reply.toJsonString());

    if (m_state == EGameState::AutoPlay) {
        autoPlay();
    }
}

void Game::handleGuess(const QString& msg) {
    auto guessData = Data::parse<GuessData>(msg);

    advanceGame(guessData->m_number);
}

void Game::autoPlay() {

    auto number = QRandomGenerator::global()->bounded(m_autoBounds.first,
                                                      m_autoBounds.second);

    advanceGame(number);

    if (m_state == EGameState::AutoPlay) {
        if (number >= m_number) {
            m_autoBounds.second = number;
        } else {
            m_autoBounds.first = number;
        }

        autoPlay();
    }
}

void Game::advanceGame(int number) {
    ECompare c;
    if (number == m_number) {
        c = ECompare::Equal;
    } else if (number <= m_number) {
        c = ECompare::Smaller;
    } else {
        c = ECompare::Bigger;
    }

    m_tries++;
    if (c == ECompare::Equal) {
        setState(EGameState::Won);
    } else if (m_triesMax != -1) {
        if (remainingTries() == 0) {
            setState(EGameState::Lost);
        }
    }

    GuessReplyData reply;
    reply.m_compare = c;
    reply.m_triesLeft = (m_triesMax == -1) ? -1 : remainingTries();
    reply.m_gameState = m_state;
    if ((m_state == EGameState::Won) || (m_state == EGameState::Lost)
        || (m_state == EGameState::Abondoned)) {
        reply.m_number = m_number;
    }
    reply.m_tries = m_tries;
    m_socket->sendTextMessage(reply.toJsonString());
}

void Game::setState(EGameState state) {
    m_state = state;
    emit stateChanged(m_state);
}
