#ifndef DRAWVISITORCREATOR_H
#define DRAWVISITORCREATOR_H

#include "drawvisitor.h"
#include <memory>

class DrawVisitorCreator {
public:
  std::shared_ptr<DrawVisitor> createVisitor();

private:
  void createInstance();

  std::shared_ptr<DrawVisitor> _visitor;
};

#endif // DRAWVISITORCREATOR_H
