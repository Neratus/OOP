#ifndef DOOR_H
#define DOOR_H

#include <QDebug>
#include <QObject>
#include <QTimer>

#include "config.h"

class Door : public QObject {
  Q_OBJECT;

  enum doorStatus { CLOSING, CLOSED, OPENING, OPENED };

public:
  explicit Door(QObject *parent = nullptr);
  ~Door() = default;

public slots:
  void closing();
  void closed();
  void requestOpening();
  void opened();

signals:
  void isClosed();
  void stateChanged(QString status);

private:
  void extendOpened();
  void opening();

  doorStatus _state;

  QTimer _openingTimer;
  QTimer _closingTimer;
  QTimer _openedTimer;
};

#endif // DOOR_H
