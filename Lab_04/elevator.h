#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "config.h"
#include <QObject>
#include <QTimer>

#include "door.h"

class Elevator : public QObject {
  Q_OBJECT;

  enum cabinStatus { FREE, WAIT, MOVING };

public:
  explicit Elevator(QObject *parent = nullptr);
  ~Elevator() = default;

  size_t currentFloor() const;
  bool canHandle(size_t floor) const;
  direction getDirection() const;
  size_t getTarget() const;

public slots:
  void onRequestElevator(size_t floor, direction dir);
  void chooseState();
  void moving();
  void stopped(bool isLast, size_t newFloor);

signals:
  void targetChosen(size_t floor, direction dir);
  void floorHandled(Elevator *sender, size_t floor, direction dir);

  void floorChanged(size_t floor);
  void doorStateChanged(const QString &state);
  void cabinStateChanged(const QString &state);

  void passFloor(Elevator *sender, size_t floor, direction dir);
  void stop(bool isLast, size_t newFloor = 1);
  void move(Elevator *sender);
  void openDoor();

private:
  cabinStatus _state;
  Door _door;
  size_t _curFloor;
  size_t _needFloor;
  bool _newTarget;
  direction _dir;
  QTimer _moveTimer;
};

#endif