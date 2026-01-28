#include "modelloadmoderatorcreator.h"
#include "csvmodelloader.h"
#include "filemodelloader.h"
#include <stdexcept>

std::shared_ptr<ModelLoadModerator>
ModelLoadModeratorCreator::createModerator() {
  if (!_moderator) {
    auto loader = std::make_shared<FileModelLoader>();
    createInstance(loader);
  }
  return _moderator;
}

std::shared_ptr<ModelLoadModerator>
ModelLoadModeratorCreator::createModerator(const std::string &fileName) {
  if (!_moderator) {
    auto loader = createLoaderByExtension(fileName);
    createInstance(loader);
  } else {
    auto loader = createLoaderByExtension(fileName);
    _moderator->setLoader(loader);
  }

  return _moderator;
}

void ModelLoadModeratorCreator::createInstance(
    const std::shared_ptr<BaseModelLoader> &loader) {
  _moderator = std::make_shared<ModelLoadModerator>(loader);
}

std::shared_ptr<BaseModelLoader>
ModelLoadModeratorCreator::createLoaderByExtension(
    const std::string &fileName) {
  auto extPos = fileName.rfind('.');
  if (extPos == std::string::npos)
    throw std::runtime_error("File extension not found");

  std::string ext = fileName.substr(extPos + 1);

  if (ext == "txt")
    return std::make_shared<FileModelLoader>();
  else if (ext == "csv")
    return std::make_shared<CSVModelLoader>();
  else
    throw std::runtime_error("Unsupported file format: " + ext);
}
