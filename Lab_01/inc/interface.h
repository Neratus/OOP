#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>

#include "err.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MOVEMENT_SPEED 5

#define TEMP_FILE "temp.txt"

err_t init(SDL_Window **window, SDL_Renderer **renderer);

err_t run(SDL_Renderer *renderer);

void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif
