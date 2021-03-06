#ifndef SERVER_H
#define SERVER_H

#include "data.h"
#include <QObject>

class Game;
class QWebSocketServer;

class Server : public QObject {
public:
  Server(int port, int tries, bounds_t bounds);
  ~Server();

  void startListen();
  void stopListen();

private:
  void addGame();

  int m_port;
  int m_triesMax;
  bounds_t m_bounds;
  QWebSocketServer *const m_websrv;
  std::vector<std::shared_ptr<Game>> m_games;
};

#endif // SERVER_H
