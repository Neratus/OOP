#ifndef EDGES_H
#define EDGES_H

#include "draw.h"
#include "err.h"
#include "points.h"

struct edge_t {
  size_t p1;
  size_t p2;
};

struct edges_t {
  edge_t *arr;
  size_t len;
};

void edges_init(edges_t &edges);

err_t edges_input(edges_t &edges, FILE *f);

err_t edges_check(edges_t &edges, size_t &points_cnt);

err_t edges_draw(const scene_t &scene, const edges_t &edges,
                 const points_t &points);

void edges_free(edges_t &edges);

#endif
