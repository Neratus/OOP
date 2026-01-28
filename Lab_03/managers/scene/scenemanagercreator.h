#ifndef SCENEMANAGERCREATOR_H
#define SCENEMANAGERCREATOR_H

#include "scenemanager.h"
#include <memory>

class SceneManagerCreator {
public:
  std::shared_ptr<SceneManager> createManager();

private:
  void createInstance();

  std::shared_ptr<SceneManager> _manager;
};

#endif // SCENEMANAGERCREATOR_H
