#include "data.h"
#include "errors.h"
#include <iostream>

#include "client.h"
#include "game.h"

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  std::cout << "client" << std::endl;
  QApplication app(argc, argv);

//  Client client("akad", Client::default_host, Client::default_port);

//  QObject::connect(
//      &client, &Client::gameCreated, &app, [&](std::shared_ptr<Game> game) {
//        qDebug() << "game created";
//        QObject::connect(game.get(), &Game::updated, &app, [&] {
//          qDebug() << (int)(game->state()) << (int)(game->lastCompare());
//        });
//      });

//  client.createGame(true);

  MainWindow w;
  w.show();
  return app.exec();
}
