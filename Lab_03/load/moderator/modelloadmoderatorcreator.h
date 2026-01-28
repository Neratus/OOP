#ifndef MODELLOADMODERATORCREATOR_H
#define MODELLOADMODERATORCREATOR_H

#include "modelloadmoderator.h"
#include <memory>
#include <string>

class ModelLoadModeratorCreator {
public:
  std::shared_ptr<ModelLoadModerator> createModerator();

  std::shared_ptr<ModelLoadModerator>
  createModerator(const std::string &fileName);

private:
  std::shared_ptr<ModelLoadModerator> _moderator;

  void createInstance(const std::shared_ptr<BaseModelLoader> &loader);

  std::shared_ptr<BaseModelLoader>
  createLoaderByExtension(const std::string &fileName);
};

#endif // MODELLOADMODERATORCREATOR_H
