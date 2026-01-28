#include <cmath>

#include "points.h"

size_t points_get_len(const points_t &points) { return points.len; }

void set_point(point_t &dest, double x, double y, double z) {
  dest.x = x;
  dest.y = y;
  dest.z = z;
}

void copy_point(point_t &dest, point_t &src) {
  dest.x = src.x;
  dest.y = src.y;
  dest.z = src.z;
}

void points_init(points_t &points) {
  points.arr = NULL;
  points.len = 0;
}

static err_t points_read_len(size_t &len, FILE *f) {
  err_t rc = OK;
  size_t size;

  if (fscanf(f, "%zu", &size) != 1)
    rc = ERR_INPUT;
  else if (size < 2)
    rc = ERR_SIZE;
  len = size;
  return rc;
}

static err_t points_alloc(points_t &point, size_t &len) {
  err_t rc = OK;

  point_t *arr = (point_t *)malloc(sizeof(point_t) * len);
  if (arr == nullptr)
    rc = ERR_MALLOC;
  point.arr = arr;
  point.len = len;

  return rc;
}

static err_t point_read(point_t &point, FILE *f) {
  err_t rc = OK;

  double x, y, z;
  if (fscanf(f, "%lf %lf %lf", &x, &y, &z) != 3)
    rc = ERR_INPUT;

  point.x = x;
  point.y = y;
  point.z = z;

  return rc;
}

static err_t points_arr_read(point_t *arr, size_t len, FILE *f) {
  err_t rc = OK;

  for (size_t i = 0; i < len; i++)
    rc = point_read(arr[i], f);

  return rc;
}

static void points_arr_free(point_t *arr) { free(arr); }

err_t points_input(points_t &points, FILE *f) {
  if ((f == NULL) || (points.arr != NULL))
    return ERR_PARAM;

  err_t rc = OK;

  rc = points_read_len(points.len, f);

  if (rc == OK) {
    rc = points_alloc(points, points.len);
    if (rc == OK) {
      rc = points_arr_read(points.arr, points.len, f);
      if (rc != OK)
        points_arr_free(points.arr);
    }
  }
  return rc;
}

static void point_move(point_t &point, const move_t coefs) {
  point.x += coefs.dx;
  point.y += coefs.dy;
  point.z += coefs.dz;
}

err_t points_move(points_t &points, const move_t coefs) {
  if (points.arr == NULL)
    return ERR_PARAM;

  for (size_t i = 0; i < points.len; i++)
    point_move(points.arr[i], coefs);

  return OK;
}

static void point_scale(point_t &point, const point_t &center,
                        const scale_t &coefs) {
  point.x = center.x + coefs.kx * (point.x - center.x);
  point.y = center.y + coefs.ky * (point.y - center.y);
  point.z = center.z + coefs.kz * (point.z - center.z);
}

err_t points_scale(points_t &points, const point_t &center,
                   const scale_t &coefs) {
  if (points.arr == NULL)
    return ERR_PARAM;

  for (size_t i = 0; i < points.len; i++)
    point_scale(points.arr[i], center, coefs);

  return OK;
}

static double deg_to_rad(double angle) { return angle * M_PI / 180.0; }

static void set_coords_local(point_t &point, const point_t &center) {
  move_t local_scale = {-center.x, -center.y, -center.y};
  point_move(point, local_scale);
}

static void set_coords_global(point_t &point, const point_t &center) {
  move_t global_scale = {center.x, center.y, center.y};
  point_move(point, global_scale);
}

static void rotate_around_x(point_t &point, double angle) {
  angle = deg_to_rad(angle);
  double y = point.y * cos(angle) - point.z * sin(angle);
  double z = point.y * sin(angle) + point.z * cos(angle);
  point.y = y;
  point.z = z;
}

static void rotate_around_y(point_t &point, double angle) {
  angle = deg_to_rad(angle);
  double x = point.x * cos(angle) + point.z * sin(angle);
  double z = -point.x * sin(angle) + point.z * cos(angle);
  point.x = x;
  point.z = z;
}

static void rotate_around_z(point_t &point, double angle) {
  angle = deg_to_rad(angle);
  double x = point.x * cos(angle) - point.y * sin(angle);
  double y = point.x * sin(angle) + point.y * cos(angle);
  point.x = x;
  point.y = y;
}

static void point_rotate(point_t &point, const point_t &center,
                         const rotate_t &coefs) {
  set_coords_local(point, center);
  rotate_around_x(point, coefs.ax);
  rotate_around_y(point, coefs.ay);
  rotate_around_z(point, coefs.az);
  set_coords_global(point, center);
}

err_t points_rotate(points_t &points, const point_t &center,
                    const rotate_t &coefs) {
  if (points.arr == NULL)
    return ERR_PARAM;

  for (size_t i = 0; i < points.len; ++i)
    point_rotate(points.arr[i], center, coefs);

  return OK;
}

void points_free(points_t &points) {
  free(points.arr);
  points.arr = NULL;
  points.len = 0;
}
