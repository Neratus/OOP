#ifndef POINT_H
#define POINT_H

#include "err.h"
#include "object_pos.h"
#include <cstdlib>

struct point_t {
  double x;
  double y;
  double z;
};

struct points_t {
  point_t *arr;
  size_t len;
};

size_t points_get_len(const points_t &points);

void set_point(point_t &dest, double x, double y, double z);

void copy_point(point_t &dest, point_t &src);

void points_init(points_t &points);

err_t points_input(points_t &points, FILE *f);

err_t points_move(points_t &points, const move_t coefs);

err_t points_scale(points_t &points, const point_t &center,
                   const scale_t &coefs);

err_t points_rotate(points_t &points, const point_t &center,
                    const rotate_t &coefs);

void points_free(points_t &points);

#endif
