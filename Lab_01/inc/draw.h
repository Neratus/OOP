#ifndef DRAW_H
#define DRAW_H

#include "SDL_draw.h"
#include "err.h"
#include "points.h"

struct scene_t {
  SDL_Renderer *renderer;
  SDL_color background;
  SDL_color foreground;
  double width;
  double height;
};

void draw_line(const scene_t &scene, const point_t &p1, const point_t &p2);

void clear(const scene_t &scene);

#endif
