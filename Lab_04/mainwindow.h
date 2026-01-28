#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>

#include "controller.h"
#include "dispatcher.h"
#include "elevator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  std::unique_ptr<QHBoxLayout> _mainLayout;
  std::unique_ptr<QVBoxLayout> _leftLayout;
  std::unique_ptr<QVBoxLayout> _middleLayout;
  std::unique_ptr<QVBoxLayout> _rightLayout;

  std::unique_ptr<Controller> _controller;
  std::unique_ptr<Dispatcher> _dispatcher;

  QLabel *_floorLabel1;
  QLabel *_floorLabel2;

  QLabel *_statusLabel1;
  QLabel *_statusLabel2;

  QLabel *_doorStatusLabel1;
  QLabel *_doorStatusLabel2;

  std::unique_ptr<Elevator> _elevator1;
  std::unique_ptr<Elevator> _elevator2;
};
#endif // MAINWINDOW_H
