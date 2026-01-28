#ifndef BASELOADER_H
#define BASELOADER_H

#include <fstream>
#include <memory>
#include <string>

#include "object.h"

class BaseLoader {
public:
  BaseLoader() = default;
  virtual ~BaseLoader() = default;

  virtual void open(std::string &fileName) = 0;
  virtual void close() = 0;
};

#endif // BASE_LOADER_H
