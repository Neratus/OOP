#include "dispatcher.h"

Dispatcher::Dispatcher(Elevator &e1, Elevator &e2, QObject *parent)
    : QObject(parent), _e1(e1), _e2(e2), _state(IDLE) {
  _visitedFloorsUp.resize(MAXFLOOR, true);
  _visitedFloorsDown.resize(MAXFLOOR, true);

  connect(this, &Dispatcher::requestElevator, &_e1,
          &Elevator::onRequestElevator);
  connect(this, &Dispatcher::requestElevator, &_e2,
          &Elevator::onRequestElevator);

  connect(&_e1, &Elevator::passFloor, this, &Dispatcher::floorServed);
  connect(&_e2, &Elevator::passFloor, this, &Dispatcher::floorServed);
}

void Dispatcher::floorServed(Elevator *elevator, size_t floor, direction dir) {
  if ((_visitedFloorsUp[floor - 1] && _visitedFloorsDown[floor - 1]) ||
      (_elevatorRequests[elevator] == 0)) {
    _state = IDLE;
    return;
  }

  _elevatorRequests.erase(elevator);

  if (!_visitedFloorsUp[floor - 1] || !_visitedFloorsDown[floor - 1]) {
    emit releaseButtonSignal(floor, UP);
    emit releaseButtonSignal(floor, DOWN);
    _visitedFloorsUp[floor - 1] = true;
    _visitedFloorsDown[floor - 1] = true;
  } else if ((floor == MINFLOOR) || (floor == MAXFLOOR)) {
    emit releaseButtonSignal(floor, DOWN);
    emit releaseButtonSignal(floor, UP);
    _visitedFloorsDown[floor - 1] = true;
    _visitedFloorsUp[floor - 1] = true;
  }

  size_t nextFloor = 0;
  direction nextDir = UP;

  if (findNearestFloor(floor, dir, nextFloor, nextDir)) {
    _elevatorRequests[elevator] = nextFloor;
    elevator->onRequestElevator(nextFloor, nextDir);
  } else {
    _state = IDLE;
  }
}

void Dispatcher::handleRequest(size_t floor, direction dir) {
  _state = ACTIVE;
  auto request = std::make_pair(floor, dir);
  if (isRequestReserved(request))
    return;

  if (dir == UP)
    _visitedFloorsUp[floor - 1] = false;
  else
    _visitedFloorsDown[floor - 1] = false;

  Elevator *freeElevator1 = _e1.canHandle(floor) ? &_e1 : nullptr;
  Elevator *freeElevator2 = _e2.canHandle(floor) ? &_e2 : nullptr;

  Elevator *chosenElevator = nullptr;

  if (freeElevator1 && freeElevator2) {
    size_t bestFloor;
    direction bestDir;
    chosenElevator = &chooseElevator(floor, dir, bestFloor, bestDir);
    _elevatorRequests[chosenElevator] = bestFloor;

    chosenElevator->onRequestElevator(bestFloor, bestDir);
  } else if (freeElevator1 || freeElevator2) {
    Elevator *freeElevator = freeElevator1 ? freeElevator1 : freeElevator2;
    Elevator *busyElevator = (freeElevator == &_e1) ? &_e2 : &_e1;

    direction busyDir = busyElevator->getDirection();

    if ((busyDir == dir) &&
        ((dir == UP && busyElevator->currentFloor() < floor &&
          floor < busyElevator->getTarget()) ||
         (dir == DOWN && busyElevator->currentFloor() > floor &&
          floor > busyElevator->getTarget()))) {
      size_t bestFloor;
      direction bestDir;
      chosenElevator = busyElevator;
      bestFloor = floor;
      bestDir = dir;

      _elevatorRequests.erase(busyElevator);
      _elevatorRequests[chosenElevator] = bestFloor;
      chosenElevator->onRequestElevator(bestFloor, bestDir);
    } else if (freeElevator->canHandle(floor)) {
      size_t bestFloor;
      direction bestDir;
      chosenElevator = freeElevator;
      bestFloor = floor;
      bestDir = dir;

      _elevatorRequests[chosenElevator] = bestFloor;
      chosenElevator->onRequestElevator(bestFloor, bestDir);
    }
  } else {

    if ((_e1.getDirection() == dir) &&
        ((dir == UP && _e1.currentFloor() < floor && floor < _e1.getTarget()) ||
         (dir == DOWN && _e1.currentFloor() > floor &&
          floor > _e1.getTarget()))) {
      _elevatorRequests.erase(&_e1);
      _elevatorRequests[&_e1] = floor;
      _e1.onRequestElevator(floor, dir);
    } else if ((_e2.getDirection() == dir) &&
               ((dir == UP && _e2.currentFloor() < floor &&
                 floor < _e2.getTarget()) ||
                (dir == DOWN && _e2.currentFloor() > floor &&
                 floor > _e2.getTarget()))) {
      _elevatorRequests.erase(&_e2);
      _elevatorRequests[&_e2] = floor;
      _e2.onRequestElevator(floor, dir);
    }
  }
}

int Dispatcher::evaluateElevator(const Elevator &elevator, size_t floor,
                                 direction callDir) const {
  int elevatorFloor = static_cast<int>(elevator.currentFloor());
  int targetFloor = static_cast<int>(floor);
  direction elevatorDir = elevator.getDirection();

  int dist = std::abs(elevatorFloor - targetFloor);

  if (elevator.canHandle(floor))
    return 0;

  if (elevatorDir == callDir) {
    if (callDir == UP && elevatorFloor <= targetFloor)
      return dist;
    if (callDir == DOWN && elevatorFloor >= targetFloor)
      return dist;
  }

  return dist + 100;
}

Elevator &Dispatcher::chooseElevator(size_t floor, direction callDir,
                                     size_t &bestFloor, direction &bestDir) {
  int dist1 =
      std::abs(static_cast<int>(_e1.currentFloor()) - static_cast<int>(floor));
  int dist2 =
      std::abs(static_cast<int>(_e2.currentFloor()) - static_cast<int>(floor));

  Elevator &chosen = (dist1 <= dist2) ? _e1 : _e2;

  bestFloor = floor;
  bestDir = callDir;

  qDebug() << "Выбор ближайшего лифта:";
  qDebug() << "Лифт 1 на этаже:" << _e1.currentFloor()
           << "расстояние:" << dist1;
  qDebug() << "Лифт 2 на этаже:" << _e2.currentFloor()
           << "расстояние:" << dist2;
  qDebug() << "Выбран лифт:" << &chosen;

  return chosen;
}

bool Dispatcher::findNearestFloor(size_t curFloor, direction curDir,
                                  size_t &nearestFloor, direction &nearestDir) {
  std::set<size_t> combinedRequests;
  if (_elevatorRequests.count(&_e1))
    combinedRequests.insert(_elevatorRequests[&_e1]);
  if (_elevatorRequests.count(&_e2))
    combinedRequests.insert(_elevatorRequests[&_e2]);

  auto skip = [&](size_t f, direction) {
    return combinedRequests.count(f) > 0;
  };

  if (curDir == UP) {
    for (size_t i = curFloor + 1; i <= MAXFLOOR; ++i) {
      if (!_visitedFloorsUp[i - 1] && !skip(i, UP)) {
        nearestFloor = i;
        nearestDir = UP;
        return true;
      }
    }
    for (int i = MAXFLOOR; i >= (int)MINFLOOR; --i) {
      if (!_visitedFloorsDown[i - 1] && !skip(i, DOWN)) {
        nearestFloor = i;
        nearestDir = DOWN;
        return true;
      }
    }
    for (int i = MAXFLOOR; i >= (int)MINFLOOR; --i) {
      if (!_visitedFloorsUp[i - 1] && !skip(i, UP)) {
        nearestFloor = i;
        nearestDir = UP;
        return true;
      }
    }
  } else if (curDir == DOWN) {
    for (int i = curFloor - 1; i >= (int)MINFLOOR; --i) {
      if (!_visitedFloorsDown[i - 1] && !skip(i, DOWN)) {
        nearestFloor = i;
        nearestDir = DOWN;
        return true;
      }
    }
    for (size_t i = MINFLOOR; i <= MAXFLOOR; ++i) {
      if (!_visitedFloorsUp[i - 1] && !skip(i, UP)) {
        nearestFloor = i;
        nearestDir = UP;
        return true;
      }
    }
    for (size_t i = MINFLOOR; i <= MAXFLOOR; ++i) {
      if (!_visitedFloorsDown[i - 1] && !skip(i, DOWN)) {
        nearestFloor = i;
        nearestDir = DOWN;
        return true;
      }
    }
  } else {
    for (size_t i = MINFLOOR; i <= MAXFLOOR; ++i) {
      if (!_visitedFloorsUp[i - 1] && !skip(i, UP)) {
        nearestFloor = i;
        nearestDir = UP;
        return true;
      }
      if (!_visitedFloorsDown[i - 1] && !skip(i, DOWN)) {
        nearestFloor = i;
        nearestDir = DOWN;
        return true;
      }
    }
  }
  return false;
}

bool Dispatcher::isRequestReserved(
    const std::pair<size_t, direction> &request) const {
  size_t floorToCheck = request.first;
  for (const auto &[elevator, floor] : _elevatorRequests) {
    if (floor == floorToCheck) {
      return true;
    }
  }
  return false;
}
