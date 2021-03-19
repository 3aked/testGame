#include "clientdefwidget.h"

#include "client.h"

ClientDefWidget::ClientDefWidget(QWidget *parent)
    : QWidget(parent), Ui::ClientDefWidget() {
  setupUi(this);

  hostLineEdit->setText(Client::default_host);
  portSpinBox->setValue(Client::default_port);
  connect(checkBox_autoPlay, &QCheckBox::clicked, this,
    [this]{nameLabel->setEnabled(false);
  } );

  errorLabel->setStyleSheet("QLabel {  color : red; }");
}

ClientDefWidget::~ClientDefWidget() {}

std::shared_ptr<Client> ClientDefWidget::getClient() const {

  return std::make_shared<Client>(nameLineEdit->text(), hostLineEdit->text(),
                                  portSpinBox->value());
}
