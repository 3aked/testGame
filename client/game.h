#ifndef GAME_H
#define GAME_H

#include "data.h"
#include "errors.h"
#include <QObject>

class QWebSocket;

class Game : public QObject {
  Q_OBJECT
public:
  Game(QWebSocket *socket, bool autoplay, int triesMax, bounds_t bounds,
       QObject *parent = nullptr);
  ~Game();

  void autoPlay();

  void play(int number);

  void abandon();

  QDateTime startDate() const { return m_startDate; }
  EGameState state() const { return m_state; }
  bounds_t autoBounds() const { return m_autoBounds; }
  int tries() const { return m_tries; }
  bounds_t bounds() const { return m_bounds; }
  int triesMax() const { return m_triesMax; }
  bool autoplay() const { return m_autoplay; }
  QWebSocket *socket() const { return m_socket; }
  ECompare lastCompare() const { return m_lastCompare; }
  int remainingTries() const { return m_triesMax - m_tries; }
  std::optional<int> servernumber() const { return m_servernumber; }
  int lastNumber() const { return m_lastGuessData.m_number; }

private:
  void handleNewMessage(const QString &message);
  void handleErrorData(const ErrorData &errorData);
  void handleGuessReplyData(const GuessReplyData &guessReplyData);
  void setState(EGameState state);

  QWebSocket *const m_socket;
  bool m_autoplay;
  int m_triesMax;
  bounds_t m_bounds;
  GuessData m_lastGuessData;

  int m_tries;
  bounds_t m_autoBounds;
  EGameState m_state;
  QDateTime m_startDate;
  ECompare m_lastCompare;
  std::optional<int> m_servernumber;
signals:
  void errorOccured(const ErrorData &);
  void stateChanged(EGameState);
  void updated();
};

#endif // GAME_H
