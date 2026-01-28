#include "door.h"

Door::Door(QObject *parent) : QObject(parent) {
  _state = CLOSED;

  _openingTimer.setSingleShot(true);
  _closingTimer.setSingleShot(true);
  _openedTimer.setSingleShot(true);

  connect(&_openingTimer, &QTimer::timeout, this, &Door::opened);
  connect(&_openedTimer, &QTimer::timeout, this, &Door::closing);
  connect(&_closingTimer, &QTimer::timeout, this, &Door::closed);
}

void Door::closing() {
  if (OPENED == _state) {
    _state = CLOSING;
    emit stateChanged("Закрываются");
    _closingTimer.start(MOVINGDOORTIME);
  }
}

void Door::closed() {
  if (CLOSING == _state) {
    emit stateChanged("Закрыты");
    _state = CLOSED;
    emit isClosed();
  }
}

void Door::requestOpening() {
  if (_state == OPENED) {
    extendOpened();
  } else if (_state == CLOSING || _state == CLOSED || _state == OPENING) {
    opening();
  }
}

void Door::opening() {
  emit stateChanged("Открываются");
  if (_state == CLOSED) {
    _state = OPENING;
    _openingTimer.start(MOVINGDOORTIME);
  } else if (_state == CLOSING) {
    _state = OPENING;
    int elapsedTime = _closingTimer.remainingTime();
    _closingTimer.stop();
    _openingTimer.start(MOVINGDOORTIME - elapsedTime);
  }
}

void Door::extendOpened() {
  emit stateChanged("Открыты (удерживаются)");
  _openedTimer.stop();
  _openedTimer.start(OPENEDTIME);
}

void Door::opened() {
  if (OPENING == _state) {
    _state = OPENED;
    emit stateChanged("Открыты");

    _openedTimer.start(OPENEDTIME);
  }
}
