#ifndef BUTTON_H
#define BUTTON_H

#include "config.h"
#include <QDebug>
#include <QObject>
#include <QPushButton>

class Button : public QPushButton {
  Q_OBJECT;

  enum btnStatus { PRESSED, UNPRESSED };

public:
  Button(QWidget *parent = nullptr);
  ~Button() = default;

  void setFloor(const size_t floor);

  void setDirection(direction dir);
  direction getDirection() const;

public slots:
  void pressed();
  void unpressed();

signals:
  void floorCall(const size_t floor, direction direction);
  void unpress();

private:
  btnStatus _state;
  direction _direction;
  size_t _floor;
};

#endif // BUTTON_H
