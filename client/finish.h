#ifndef FINISH_H
#define FINISH_H

#include <QWidget>
#include "ui_finish.h"


class Client;
class Game;

//todo

class Finish : public QWidget, public Ui::Finish
{
    Q_OBJECT

public:
    explicit Finish(QWidget *parent = nullptr);
    ~Finish();

private:

};

#endif // FINISH_H
