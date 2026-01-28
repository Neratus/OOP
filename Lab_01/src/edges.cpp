#include "edges.h"

void edges_init(edges_t &edges) {
  edges.arr = NULL;
  edges.len = 0;
}

static err_t edges_read_len(size_t &len, FILE *f) {
  err_t rc = OK;
  size_t size;

  if (fscanf(f, "%zu", &size) != 1)
    rc = ERR_INPUT;
  else if (size == 0)
    rc = ERR_SIZE;
  len = size;
  return rc;
}

static err_t edges_alloc(edges_t &edges, size_t &len) {
  err_t rc = OK;

  edge_t *arr = (edge_t *)malloc(sizeof(edge_t) * len);
  if (arr == nullptr)
    rc = ERR_MALLOC;
  edges.arr = arr;
  edges.len = len;

  return rc;
}

static err_t edge_read(edge_t &edge, FILE *f) {
  err_t rc = OK;

  size_t p1, p2;
  if (fscanf(f, "%zu %zu", &p1, &p2) != 2)
    rc = ERR_INPUT;

  edge.p1 = p1;
  edge.p2 = p2;

  return rc;
}

static err_t edges_arr_read(edge_t *arr, size_t len, FILE *f) {
  err_t rc = OK;

  for (size_t i = 0; i < len; i++)
    rc = edge_read(arr[i], f);

  return rc;
}

static void edges_arr_free(edge_t *arr) { free(arr); }

err_t edges_input(edges_t &edges, FILE *f) {
  if ((f == NULL) || (edges.arr != NULL))
    return ERR_PARAM;

  err_t rc = OK;

  rc = edges_read_len(edges.len, f);

  if (rc == OK) {
    rc = edges_alloc(edges, edges.len);
    if (rc == OK) {
      rc = edges_arr_read(edges.arr, edges.len, f);
      if (rc != OK)
        edges_arr_free(edges.arr);
    }
  }
  return rc;
}

err_t edges_check(edges_t &edges, size_t &points_cnt) {
  if (edges.arr == NULL)
    return ERR_PARAM;

  err_t rc = OK;

  for (size_t i = 0; !rc && i < edges.len; i++) {
    edge_t cur = edges.arr[i];
    if ((cur.p1 >= points_cnt) || (cur.p2 >= points_cnt) || (cur.p1 == cur.p2))
      rc = ERR_SIZE;
  }

  return rc;
}

static void edge_draw(const scene_t &scene, const edge_t &edge,
                      const point_t *arr) {
  point_t p1 = arr[edge.p1];
  point_t p2 = arr[edge.p2];

  draw_line(scene, p1, p2);
}

err_t edges_draw(const scene_t &scene, const edges_t &edges,
                 const points_t &points) {
  if (edges.arr == NULL || points.arr == NULL)
    return ERR_PARAM;

  for (size_t i = 0; i < edges.len; i++)
    edge_draw(scene, edges.arr[i], points.arr);

  return OK;
}

void edges_free(edges_t &edges) {
  free(edges.arr);
  edges.arr = NULL;
  edges.len = 0;
}
