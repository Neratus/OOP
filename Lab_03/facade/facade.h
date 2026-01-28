#ifndef FACADE_H
#define FACADE_H

#include "basecommand.h"
#include <memory>

class Facade {
public:
  void execute(BaseCommand &command);
};

#endif // FACADE_H
