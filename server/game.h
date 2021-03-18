#ifndef GAME_H
#define GAME_H

#include "data.h"
#include <QDateTime>
#include <QObject>

class QWebSocket;

class Game : public QObject {
    Q_OBJECT
public:
    Game(QWebSocket* const socket, int triesMax, bounds_t bounds,
         QObject* parent);

    int remainingTries() const;

private:
    void handleNewMessage(const QString& msg);

    void handleInitialization(const QString& msg);
    void handleGuess(const QString& msg);

    void autoPlay();

    void advanceGame(int number);
    void setState(EGameState state);

    QWebSocket* const m_socket;
    int m_number;
    int m_triesMax;
    int m_tries;
    bounds_t m_bounds;
    bounds_t m_autoBounds;

    bool m_autoplay;

    EGameState m_state;
    QString m_name;
    QDateTime m_startDate;

signals:
    void aboutToClose();
    void stateChanged(EGameState);
};

#endif // GAME_H
