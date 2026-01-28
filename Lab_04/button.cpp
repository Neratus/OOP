#include "button.h"

Button::Button(QWidget *parent) : QPushButton(parent) {
  _state = UNPRESSED;
  _floor = 1;

  connect(this, &Button::clicked, this, &Button::pressed);
  connect(this, &Button::unpress, this, &Button::unpressed);
}

void Button::setDirection(direction dir) { _direction = dir; }

direction Button::getDirection() const { return _direction; }

void Button::setFloor(const size_t floor) { _floor = floor; }

void Button::pressed() {
  if (UNPRESSED == _state) {
    _state = PRESSED;
    setDisabled(true);
    setStyleSheet("background-color: rgb(255, 124, 124); color: black;");
    update();

    emit floorCall(_floor, _direction);
  }
}

void Button::unpressed() {
  if (PRESSED == _state) {
    _state = UNPRESSED;
    setDisabled(false);
    setStyleSheet("");
    update();
  }
}
