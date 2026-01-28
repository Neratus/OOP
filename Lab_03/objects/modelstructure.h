#ifndef MODELSTRUCTURE_H
#define MODELSTRUCTURE_H

#include "link.h"
#include "vertex.h"
#include <vector>

class ModelStructure {
public:
  ModelStructure() = default;

  ModelStructure(const std::vector<Vertex> &vertexes,
                 const std::vector<Link> &links);

  ~ModelStructure() = default;

  const std::vector<Vertex> &getVertexes() const;
  const std::vector<Link> &getLinks() const;
  const Vertex &getCenter() const;

  void addVertex(const Vertex &vertex);
  void addLink(const Link &link);

  void transform(const Vertex &move, const Vertex &scale, const Vertex &rotate);

private:
  Vertex _center;
  std::vector<Vertex> _vertexes;

#ifdef USE_ADJ_MATRIX
  std::vector<std::vector<int>> _adjacencyMatrix;
#else
  std::vector<Link> _links;
#endif

#ifdef USE_ADJ_MATRIX
  void buildAdjacencyMatrixFromLinks(const std::vector<Link> &links);
#endif
};

#endif // MODELSTRUCTURE_H
