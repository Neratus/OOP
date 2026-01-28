#include "controller.h"
#include "dispatcher.h"

Controller::Controller(Dispatcher *dispatcher, QWidget *parent)
    : QWidget(parent), _dispatcher(dispatcher) {
  connect(dispatcher, &Dispatcher::releaseButtonSignal, this,
          &Controller::releaseButton);

  _layout = std::make_unique<QVBoxLayout>();
  setLayout(_layout.get());

  for (int i = MINFLOOR; i <= MAXFLOOR; ++i) {
    size_t floor = MAXFLOOR + 1 - i;

    auto floorLayout = new QHBoxLayout();

    auto floorLabel = new QLabel(QString("Этаж %1").arg(floor));
    floorLabel->setFixedWidth(60);
    floorLayout->addWidget(floorLabel);

    if (floor < MAXFLOOR) {
      auto btnUp = std::make_shared<Button>();
      btnUp->setFloor(floor);
      btnUp->setDirection(UP);
      btnUp->setText("↑");

      _btns[{floor, UP}] = btnUp;
      floorLayout->addWidget(dynamic_cast<QPushButton *>(btnUp.get()));

      QObject::connect(btnUp.get(), &Button::floorCall, _dispatcher,
                       &Dispatcher::handleRequest);
    } else {
      floorLayout->addSpacing(40);
    }

    if (floor > MINFLOOR) {
      auto btnDown = std::make_shared<Button>();
      btnDown->setFloor(floor);
      btnDown->setDirection(DOWN);
      btnDown->setText("↓");

      _btns[{floor, DOWN}] = btnDown;
      floorLayout->addWidget(dynamic_cast<QPushButton *>(btnDown.get()));

      QObject::connect(btnDown.get(), &Button::floorCall, _dispatcher,
                       &Dispatcher::handleRequest);
    } else {
      floorLayout->addSpacing(40);
    }

    _layout->addLayout(floorLayout);
  }
}

void Controller::releaseButton(size_t floor, direction dir) {
  auto it = _btns.find({floor, dir});
  if (it != _btns.end())
    emit it->second->unpress();
}
