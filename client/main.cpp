#include <iostream>

#include "data.h"
#include "errors.h"

#include <QCoreApplication>
#include <QWebSocket>

#include <QDebug>

int main(int argc, char* argv[]) {
    std::cout << "client" << std::endl;
    QCoreApplication app(argc, argv);

    QWebSocket socket;

    QObject::connect(&socket, &QWebSocket::connected, [&] {
        InitializationData d;
        d.m_name = "akad";
        d.m_autoplay = true;

        socket.sendTextMessage(d.toJsonString());
    });

    QObject::connect(&socket, &QWebSocket::textMessageReceived,
                     [&](const QString& str) {
                         qDebug().noquote() << str;
                     });

    QObject::connect(
        &socket, qOverload<QAbstractSocket::SocketError>(&QWebSocket::error),
        [&] { qDebug().noquote() << socket.errorString(); });

    socket.open(QUrl::fromUserInput("ws://localhost:4242"));

    return app.exec();
}
