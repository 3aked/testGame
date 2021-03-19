#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "errors.h"

class QWebSocket;
class Game;

class Client : public QObject {
  Q_OBJECT
public:
  static QString const default_host;
  static int const default_port;

  Client(const QString &name, const QString &host, int port,
         QObject *parent = nullptr);

  void createGame(bool autoplay);

  QUrl hostUrl() const;

private:
  void handleError(const ErrorData &error);
  QString m_host;
  int m_port;
  QString m_name;

  std::vector<std::shared_ptr<Game>> m_games;
signals:
  void errorOccured(ErrorData);
  void gameCreated(std::shared_ptr<Game>);
};

#endif // CLIENT_H
