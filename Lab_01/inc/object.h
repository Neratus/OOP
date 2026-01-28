#ifndef OBJECT_H
#define OBJECT_H

#include "draw.h"
#include "edges.h"
#include "err.h"
#include "object_pos.h"
#include "points.h"

struct object_t {
  points_t points;
  edges_t edges;
};

object_t &object_init(void);

void object_copy(object_t &dst, object_t &src);

err_t object_load(object_t &obj, const char *filename);

err_t object_check(object_t &obj);

err_t object_draw(object_t &obj, const scene_t &scene);

err_t object_move(object_t &obj, const move_t &coefs);

err_t object_scale(object_t &obj, const point_t &center, const scale_t &coefs);

err_t object_rotate(object_t &obj, const point_t &center,
                    const rotate_t &coefs);

void object_free(object_t &obj);

#endif
