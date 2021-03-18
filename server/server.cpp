#include "server.h"
#include "game.h"

#include <QRandomGenerator>
#include <QWebSocket>
#include <QWebSocketServer>

#include <QDebug>
#include <exception>

#include <iostream>

Server::Server(int port, int tries, bounds_t bounds)
    : m_port{port},
      m_triesMax{tries},
      m_bounds{bounds},
      m_websrv{new QWebSocketServer("GameServer",
                                    QWebSocketServer::NonSecureMode, this)} {

    connect(m_websrv, &QWebSocketServer::newConnection, this, &Server::addGame);
    connect(m_websrv, &QWebSocketServer::closed, this, &Server::closeGames);
}

Server::~Server() { stopListen(); }

void Server::startListen() {
    if (!m_websrv->listen(QHostAddress::Any, m_port)) {
        throw std::runtime_error(m_websrv->errorString().toStdString());
    }
}

void Server::stopListen() { m_websrv->close(); }

void Server::addGame() {
    auto connection = m_websrv->nextPendingConnection();
    assert(connection);

    std::cout << "new connection from :"
              << connection->peerAddress().toString().toStdString()
              << std::endl;

    auto game = new Game(connection, m_triesMax, m_bounds, nullptr);

    connect(game, &Game::aboutToClose, this,
            [this, game] { removeGame(game); });
    m_games.push_back(game);
}

void Server::closeGames() {
    for (auto game : m_games) {
        game->deleteLater();
    }

    m_games.clear();
}

void Server::removeGame(Game* game) {

    m_games.erase(std::remove(m_games.begin(), m_games.end(), game),
                  m_games.end());

    game->deleteLater();
}
