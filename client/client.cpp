#include "client.h"

#include "data.h"
#include "game.h"
#include <QWebSocket>

QString const Client::default_host = "localhost";
int const Client::default_port = 4242;

Client::Client(const QString &name, const QString &host, int port,
               QObject *parent)
    : QObject(parent), m_name{name}, m_host{host}, m_port{port} {}

void Client::createGame(bool autoplay) {
  auto socket = new QWebSocket();

  QObject::connect(socket, &QWebSocket::connected, this, [this, socket] {
    InitializationData d;
    d.m_name = m_name;
    socket->sendTextMessage(d.toJsonString());
  });

  connect(socket, qOverload<QAbstractSocket::SocketError>(&QWebSocket::error),
          this, [socket, this] {
            handleError(ErrorData(EError::NetworkError, socket->errorString()));
          });

  QObject::connect(socket, &QWebSocket::textMessageReceived, this,
                   [this, socket, autoplay](const QString &message) {
                     try {
                       qDebug().noquote() << message;

                       auto reply =
                           Data::parse<InitializationReplyData>(message);

                       auto game = std::make_shared<Game>(socket, autoplay,
                                                          reply->m_maxTries,
                                                          reply->m_bounds);

                       m_games.push_back(game);
                       emit gameCreated(game);
                       disconnect(socket, 0, this, 0);
                     } catch (const ErrorData &error) {
                       handleError(error);
                     }
                   });

  socket->open(hostUrl());
}

QUrl Client::hostUrl() const {
  QUrl url;
  url.setScheme("ws");
  url.setHost(m_host);
  url.setPort(m_port);
  return url;
}

void Client::handleError(const ErrorData &error) {
  qDebug() << error.errorString();
  emit errorOccured(error);
}
