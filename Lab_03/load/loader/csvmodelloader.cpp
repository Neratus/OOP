#include "csvmodelloader.h"
#include "exceptions.h"
#include "modelbuilder.h"

#include <iostream>
#include <sstream>

CSVModelLoader::CSVModelLoader() { _file = std::make_shared<std::ifstream>(); }

CSVModelLoader::CSVModelLoader(std::shared_ptr<std::ifstream> &file) {
  _file = file;
}

void CSVModelLoader::open(std::string &fileName) {
  if (!_file) {
    std::string msg = "Error: File stream is not initialized";
    throw SourceException(msg);
  }

  _file->open(fileName);

  if (!_file->is_open()) {
    std::string msg = "Error: Cannot open file " + fileName;
    throw SourceException(msg);
  }
}

void CSVModelLoader::close() {
  if (!_file) {
    std::string msg = "Error: File stream is not initialized";
    throw SourceException(msg);
  }

  _file->close();
}

std::shared_ptr<Model>
CSVModelLoader::load(std::shared_ptr<ModelBuilder> builder) {
  builder->build();

  if (!_file || !_file->is_open()) {
    std::string msg = "File is not open";
    throw SourceException(msg);
  }

  std::string line;

  do {
    if (!std::getline(*_file, line)) {
      std::string msg = "Unexpected EOF while reading vertex count";
      throw SourceException(msg);
    }
  } while (line.empty() || line[0] == '#');

  int vertexNum = 0;
  try {
    vertexNum = std::stoi(line);
  } catch (...) {
    std::string msg = "Wrong vertex num format";
    throw SourceException(msg);
  }

  if (vertexNum <= 0) {
    std::string msg = "Wrong vertex num";
    throw SourceException(msg);
  }

  for (int i = 0; i < vertexNum; i++) {
    do {
      if (!std::getline(*_file, line)) {
        std::string msg = "Unexpected EOF while reading vertex data";
        throw SourceException(msg);
      }
    } while (line.empty() || line[0] == '#');

    std::istringstream ss(line);
    double x, y, z;
    char comma1, comma2;

    if (!(ss >> x >> comma1 >> y >> comma2 >> z) || comma1 != ',' ||
        comma2 != ',') {
      std::string msg = "Error parsing vertex coordinates";
      throw SourceException(msg);
    }

    builder->buildVertex(x, y, z);
  }

  do {
    if (!std::getline(*_file, line)) {
      std::string msg = "Unexpected EOF while reading links count";
      throw SourceException(msg);
    }
  } while (line.empty() || line[0] == '#');

  int linksNum = 0;
  try {
    linksNum = std::stoi(line);
  } catch (...) {
    std::string msg = "Wrong links num format";
    throw SourceException(msg);
  }

  if (linksNum <= 0) {
    std::string msg = "Wrong links num";
    throw SourceException(msg);
  }

  for (int i = 0; i < linksNum; i++) {
    do {
      if (!std::getline(*_file, line)) {
        std::string msg = "Unexpected EOF while reading link data";
        throw SourceException(msg);
      }
    } while (line.empty() || line[0] == '#');

    std::istringstream ss(line);
    int vertex1Index, vertex2Index;
    char comma;

    if (!(ss >> vertex1Index >> comma >> vertex2Index) || comma != ',') {
      std::string msg = "Error parsing link indices";
      throw SourceException(msg);
    }

    if (vertex1Index <= 0 || vertex2Index <= 0 || vertex1Index > vertexNum ||
        vertex2Index > vertexNum) {
      std::string msg = "Wrong link data";
      throw SourceException(msg);
    }

    builder->buildLink(vertex1Index, vertex2Index);
  }

  return builder->get();
}
