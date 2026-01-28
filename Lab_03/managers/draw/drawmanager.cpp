#include "drawmanager.h"
#include "camera.h"
#include "model.h"

void DrawManager::setVisitor(std::shared_ptr<Visitor> visitor) {
  _visitor = visitor;
}

void DrawManager::drawScene(std::shared_ptr<Scene> scene) {
  for (auto &elem : *scene)
    elem->accept(_visitor);
}
