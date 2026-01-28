#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDebug>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <memory>

#include "button.h"
#include "config.h"

class Dispatcher;

class Controller : public QWidget {
  Q_OBJECT;

public:
  explicit Controller(Dispatcher *dispatcher, QWidget *parent = nullptr);
  ~Controller() = default;

public slots:
  void releaseButton(size_t floor, direction dir);

private:
  std::unique_ptr<QVBoxLayout> _layout;
  std::map<std::pair<size_t, direction>, std::shared_ptr<Button>> _btns;
  Dispatcher *_dispatcher;
};

#endif // CONTROLLER_H
