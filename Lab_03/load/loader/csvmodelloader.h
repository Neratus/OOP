#ifndef CSVMODELLOADER_H
#define CSVMODELLOADER_H

#include "basemodelloader.h"
#include <fstream>
#include <memory>
#include <string>

class CSVModelLoader : public BaseModelLoader {
public:
  CSVModelLoader();
  explicit CSVModelLoader(std::shared_ptr<std::ifstream> &file);

  ~CSVModelLoader() override = default;

  void open(std::string &fileName) override;
  void close() override;

  std::shared_ptr<Model> load(std::shared_ptr<ModelBuilder> builder) override;

protected:
  std::shared_ptr<std::ifstream> _file;
};

#endif // CSVMODELLOADER_H
