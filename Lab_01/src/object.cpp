#include "object.h"

object_t &object_init(void) {
  static object_t obj;
  points_init(obj.points);
  edges_init(obj.edges);
  return obj;
}

static err_t object_read_from_file(object_t &obj, FILE *f) {
  err_t rc = points_input(obj.points, f);
  if (rc == OK) {
    rc = edges_input(obj.edges, f);
    if (rc != OK)
      points_free(obj.points);
  }

  return rc;
}

void object_copy(object_t &dst, object_t &src) { dst = src; }

err_t object_load(object_t &obj, const char *filename) {
  if (!filename)
    return ERR_PARAM;

  FILE *f = fopen(filename, "r");
  if (f == NULL)
    return ERR_FILE;

  object_t tmp = object_init();

  err_t rc = object_read_from_file(tmp, f);
  fclose(f);

  if (rc == OK) {
    rc = object_check(tmp);
    if (rc != OK)
      object_free(tmp);
    else {
      object_free(obj);
      object_copy(obj, tmp);
    }
  }

  return rc;
}

err_t object_check(object_t &obj) {
  size_t len = points_get_len(obj.points);
  err_t rc = edges_check(obj.edges, len);
  return rc;
}

err_t object_draw(object_t &obj, const scene_t &scene) {
  clear(scene);
  return edges_draw(scene, obj.edges, obj.points);
}

err_t object_move(object_t &obj, const move_t &coefs) {
  return points_move(obj.points, coefs);
}

err_t object_scale(object_t &obj, const point_t &center, const scale_t &coefs) {
  return points_scale(obj.points, center, coefs);
}

err_t object_rotate(object_t &obj, const point_t &center,
                    const rotate_t &coefs) {
  return points_rotate(obj.points, center, coefs);
}

void object_free(object_t &obj) {
  points_free(obj.points);
  edges_free(obj.edges);
}
