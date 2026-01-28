#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <set>

#include "elevator.h"

class Dispatcher : public QObject {
  Q_OBJECT

public:
  enum dispatcherStatus { IDLE, ACTIVE };
  Dispatcher(Elevator &e1, Elevator &e2, QObject *parent = nullptr);

signals:
  void releaseButtonSignal(size_t floor, direction dir);
  void requestElevator(size_t floor, direction dir);
  void stayOnFloor(bool isLast, direction dir, size_t floor);

public slots:
  void handleRequest(size_t floor, direction dir);
  void floorServed(Elevator *elevator, size_t floor, direction dir);

private:
  std::map<Elevator *, size_t> _elevatorRequests;

  std::vector<bool> _visitedFloorsUp;
  std::vector<bool> _visitedFloorsDown;

  Elevator &_e1;
  Elevator &_e2;

  dispatcherStatus _state;

  Elevator &chooseElevator(size_t floor, direction callDir, size_t &bestFloor,
                           direction &bestDir);
  bool findNearestFloor(size_t curFloor, direction curDir, size_t &nearestFloor,
                        direction &nearestDir);
  int evaluateElevator(const Elevator &elevator, size_t floor,
                       direction callDir) const;
  bool isRequestReserved(const std::pair<size_t, direction> &request) const;
};

#endif