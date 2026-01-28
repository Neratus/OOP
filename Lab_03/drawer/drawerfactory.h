#ifndef DRAWERFACTORY_H
#define DRAWERFACTORY_H

#include "basedrawer.h"
#include <memory>

class DrawerFactory {
public:
  virtual std::unique_ptr<BaseDrawer> createDrawer() = 0;
};

#endif // DRAWERFACTORY_H
