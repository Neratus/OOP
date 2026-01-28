#include "elevator.h"

Elevator::Elevator(QObject *parent)
    : QObject(parent), _state(FREE), _curFloor(1), _needFloor(1),
      _newTarget(false), _dir(STOP) {
  _moveTimer.setSingleShot(true);

  connect(&_door, &Door::isClosed, this, &Elevator::chooseState);
  connect(&_moveTimer, &QTimer::timeout, this, &Elevator::moving);
  connect(this, &Elevator::stop, this, &Elevator::stopped);
  connect(this, &Elevator::move, this, &Elevator::moving);
  connect(this, &Elevator::openDoor, &_door, &Door::requestOpening);
  connect(&_door, &Door::stateChanged, this,
          [this](const QString &s) { emit doorStateChanged(s); });
}

void Elevator::onRequestElevator(size_t floor, direction dir) {
  _needFloor = floor;
  _dir = _curFloor < _needFloor ? UP : (_curFloor == _needFloor ? STOP : DOWN);
  if (_dir == STOP) {
    _state = WAIT;
    _newTarget = (_curFloor == _needFloor);
    emit passFloor(this, _curFloor, _dir);
    emit openDoor();
  } else if (_state == FREE) {
    _newTarget = true;
    _state = WAIT;
    emit move(this);
  }
}

void Elevator::chooseState() {
  if (_newTarget) {
    _state = WAIT;
    emit move(this);
  } else {
    emit cabinStateChanged("Ожидание");
    _state = FREE;
  }
}

void Elevator::moving() {
  emit cabinStateChanged("Движется");
  if (_state == WAIT) {
    _state = MOVING;
  } else if (_state == MOVING)
    _curFloor += _dir;

  emit floorChanged(_curFloor);

  if (_curFloor == _needFloor) {
    emit passFloor(this, _curFloor, _dir);
    emit stop(_needFloor == _curFloor, _needFloor);
  } else {
    _moveTimer.start(MOVECABINTIME);
  }
}

void Elevator::stopped(bool isLast, size_t newFloor) {
  emit cabinStateChanged("Остановлена");
  _newTarget = !isLast;
  _needFloor = newFloor;
  _dir = _curFloor < _needFloor ? UP : (_curFloor == _needFloor ? STOP : DOWN);
  _state = WAIT;
  emit floorChanged(_curFloor);
  emit openDoor();
}

size_t Elevator::currentFloor() const { return _curFloor; }

bool Elevator::canHandle(size_t floor) const {
  return _state == FREE || _curFloor == floor;
}

direction Elevator::getDirection() const { return _dir; }

size_t Elevator::getTarget() const { return _needFloor; }
