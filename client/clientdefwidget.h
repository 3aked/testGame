#ifndef CLIENTDEFWIDGET_H
#define CLIENTDEFWIDGET_H

#include "ui_clientdefwidget.h"
#include <QWidget>

class Client;

class ClientDefWidget : public QWidget, public Ui::ClientDefWidget {
  Q_OBJECT

public:
  explicit ClientDefWidget(QWidget *parent = nullptr);
  ~ClientDefWidget();

  std::shared_ptr<Client> getClient() const;
};

#endif // CLIENTDEFWIDGET_H
