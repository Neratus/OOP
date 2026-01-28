#include "modelstructure.h"

ModelStructure::ModelStructure(const std::vector<Vertex> &vertexes,
                               const std::vector<Link> &links)
    : _center{}, _vertexes(vertexes) {
#ifdef USE_ADJ_MATRIX
  _adjacencyMatrix.resize(vertexes.size(),
                          std::vector<int>(vertexes.size(), 0));
  buildAdjacencyMatrixFromLinks(links);
#else
  _links = links;
#endif
}

const std::vector<Vertex> &ModelStructure::getVertexes() const {
  return _vertexes;
}

const std::vector<Link> &ModelStructure::getLinks() const {
#ifndef USE_ADJ_MATRIX
  return _links;
#else
  static std::vector<Link> generatedLinks;
  generatedLinks.clear();

  std::size_t n = _adjacencyMatrix.size();
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = i + 1; j < n; ++j) {
      if (_adjacencyMatrix[i][j]) {
        generatedLinks.emplace_back(i + 1, j + 1);
      }
    }
  }

  return generatedLinks;
#endif
}

const Vertex &ModelStructure::getCenter() const { return _center; }

void ModelStructure::addVertex(const Vertex &vertex) {
  _vertexes.push_back(vertex);
#ifdef USE_ADJ_MATRIX
  size_t newSize = _vertexes.size();
  for (auto &row : _adjacencyMatrix)
    row.resize(newSize, 0);
  _adjacencyMatrix.emplace_back(newSize, 0);
#endif
}

void ModelStructure::addLink(const Link &link) {
#ifndef USE_ADJ_MATRIX
  _links.push_back(link);
#else
  std::size_t from = link.getFirst();
  std::size_t to = link.getSecond();

  if (from > 0)
    from--;
  if (to > 0)
    to--;

  if (from < _vertexes.size() && to < _vertexes.size()) {
    _adjacencyMatrix[from][to] = 1;
    _adjacencyMatrix[to][from] = 1;
  }
#endif
}

void ModelStructure::transform(const Vertex &move, const Vertex &scale,
                               const Vertex &rotate) {
  _center.move(move.getX(), move.getY(), move.getZ());
  for (auto &vertex : _vertexes) {
    vertex.scale(scale.getX(), scale.getY(), scale.getZ());
    vertex.rotate(rotate.getX(), rotate.getY(), rotate.getZ());
  }
}

#ifdef USE_ADJ_MATRIX
void ModelStructure::buildAdjacencyMatrixFromLinks(
    const std::vector<Link> &links) {
  for (const auto &link : links) {
    std::size_t from = link.getFirst();
    std::size_t to = link.getSecond();
    if (from < _vertexes.size() && to < _vertexes.size()) {
      _adjacencyMatrix[from][to] = 1;
      _adjacencyMatrix[to][from] = 1;
    }
  }
}
#endif
