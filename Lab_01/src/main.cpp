#include "interface.h"

int main(void) {
  SDL_Window *window;
  SDL_Renderer *renderer;

  err_t rc = init(&window, &renderer);
  if (rc != OK)
    print_err(rc);

  rc = run(renderer);
  cleanup(window, renderer);
  return rc;
}
