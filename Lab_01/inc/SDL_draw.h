#ifndef SDL_DRAW_H
#define SDL_DRAW_H

#include <SDL2/SDL.h>

enum SDL_color { SDL_WHITE, SDL_BLACK, SDL_RED, SDL_GREEN, SDL_BLUE };

void set_color_canvas(SDL_Renderer *renderer, SDL_color color_name);

void clear_canvas(SDL_Renderer *renderer);

void draw_line_canvas(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);

#endif
