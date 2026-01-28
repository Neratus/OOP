#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGroupBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(700, 500);

  _mainLayout = std::make_unique<QHBoxLayout>();
  _leftLayout = std::make_unique<QVBoxLayout>();
  _middleLayout = std::make_unique<QVBoxLayout>();
  _rightLayout = std::make_unique<QVBoxLayout>();

  ui->centralwidget->setLayout(_mainLayout.get());

  auto group1 = new QGroupBox("Лифт 1");
  auto group2 = new QGroupBox("Лифт 2");
  auto controllerGroup = new QGroupBox("Панель вызова");

  _floorLabel1 = new QLabel("Этаж: 1");
  _statusLabel1 = new QLabel("Статус: Ожидание");
  _doorStatusLabel1 = new QLabel("Дверь: Закрыта");

  _floorLabel2 = new QLabel("Этаж: 1");
  _statusLabel2 = new QLabel("Статус: Ожидание");
  _doorStatusLabel2 = new QLabel("Дверь: Закрыта");

  group1->setLayout(_leftLayout.get());
  group2->setLayout(_rightLayout.get());
  controllerGroup->setLayout(_middleLayout.get());

  _elevator1 = std::make_unique<Elevator>(this);
  _elevator2 = std::make_unique<Elevator>(this);

  _dispatcher = std::make_unique<Dispatcher>(*_elevator1, *_elevator2, nullptr);

  _controller = std::make_unique<Controller>(_dispatcher.get(), this);
  _middleLayout->addWidget(_controller.get());

  _leftLayout->addWidget(_floorLabel1);
  _leftLayout->addWidget(_statusLabel1);
  _leftLayout->addWidget(_doorStatusLabel1);

  _rightLayout->addWidget(_floorLabel2);
  _rightLayout->addWidget(_statusLabel2);
  _rightLayout->addWidget(_doorStatusLabel2);

  _mainLayout->addWidget(group1);
  _mainLayout->addWidget(controllerGroup);
  _mainLayout->addWidget(group2);

  connect(_elevator1.get(), &Elevator::floorChanged, this,
          [=](size_t f) { _floorLabel1->setText(QString("Этаж: %1").arg(f)); });

  connect(_elevator1.get(), &Elevator::cabinStateChanged, this,
          [=](const QString &s) {
            qDebug()
                << "\n MainWindow: Elevator 1 doorStateChanged signal received:"
                << s;
            _statusLabel1->setText("Статус: " + s);
          });

  connect(_elevator1.get(), &Elevator::doorStateChanged, this,
          [=](const QString &s) {
            qDebug()
                << "\n MainWindow: Elevator 1 doorStateChanged signal received:"
                << s;
            _doorStatusLabel1->setText("Дверь: " + s);
          });

  connect(_elevator2.get(), &Elevator::floorChanged, this,
          [=](size_t f) { _floorLabel2->setText(QString("Этаж: %1").arg(f)); });

  connect(
      _elevator2.get(), &Elevator::cabinStateChanged, this,
      [=](const QString &s) {
        qDebug()
            << "\n MainWindow: Elevator 1 cabinStateChanged signal received:"
            << s;
        _statusLabel2->setText("Статус: " + s);
      });

  connect(_elevator2.get(), &Elevator::doorStateChanged, this,
          [=](const QString &s) {
            qDebug()
                << "\n MainWindow: Elevator 2 doorStateChanged signal received:"
                << s;
            _doorStatusLabel2->setText("Дверь: " + s);
          });

  _floorLabel1->setFixedWidth(150);
  _statusLabel1->setFixedWidth(150);
  _doorStatusLabel1->setFixedWidth(150);

  _floorLabel2->setFixedWidth(150);
  _statusLabel2->setFixedWidth(150);
  _doorStatusLabel2->setFixedWidth(150);

  group1->setFixedWidth(150);
  controllerGroup->setFixedWidth(300);
  group2->setFixedWidth(150);
}

MainWindow::~MainWindow() { delete ui; }
