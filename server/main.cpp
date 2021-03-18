#include "server.h"

#include <QCommandLineParser>
#include <QCoreApplication>

#include <iostream>
#include <optional>

int main(int argc, char* argv[]) {

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("GameServer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Game server");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption(QStringList() << "p"
                                                << "port",
                                  "Port sur lequel attendre les clients");
    parser.addOption(portOption);

    QCommandLineOption limitOption(
        QStringList() << "l"
                      << "limit",
        "Limite de tentatives par partie (-1 pour illimitées).");
    parser.addOption(limitOption);

    QCommandLineOption boundsOption(
        QStringList() << "b"
                      << "bounds",
        "Bornes de l'interval (entiers signés, X et Y inclus)");
    parser.addOption(boundsOption);

    parser.process(app);

    int port = 4242;
    int tries = -1;
    bounds_t bounds = {1, 100};

    bool ok;

    if (parser.isSet(portOption)) {
        int vport = parser.value(portOption).toInt(&ok);
        if (ok) {
            port = vport;
        }
    }

    if (parser.isSet(limitOption)) {
        int vlimit = parser.value(limitOption).toInt(&ok);
        if (ok) {
            tries = vlimit;
        }
    }

    if (parser.isSet(boundsOption)) {
        const QStringList& boundsStrings
            = parser.value(boundsOption).split(",");
        if (boundsStrings.size() == 2) {
            bounds_t vbounds;
            vbounds.first = boundsStrings[0].toInt(&ok);
            bool ok2;
            vbounds.second = boundsStrings[0].toInt(&ok2);
            if (ok && ok2) {
                bounds = vbounds;
            }
        }
    }

    Server srv(port, tries, bounds);
    try {
        srv.startListen();

        std::cout << "Serveur écoute sur: " << port << std::endl;
        std::cout << "Bornes de l'interval: {" << bounds.first << ","
                  << bounds.second << "}" << std::endl;
        std::cout << "Limite de tentatives par partie: " << tries << std::endl;

    } catch (const std::exception &e) {
        std::cout << e.what();
        return -1;
    }

    return app.exec();

}
