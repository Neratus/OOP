#include "interface.h"

#include "draw.h"
#include "object_pos.h"
#include "task_mngr.h"

static struct {
  point_t center;
  move_t move_coef;
  scale_t scale_coef;
  rotate_t rotate_coef;
} ui;

static void ui_reset(void) {
  reset_move(ui.move_coef);
  reset_scale(ui.scale_coef);
  reset_rotate(ui.rotate_coef);
}

err_t init(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return ERR_INTERFACE;
  }

  *window = SDL_CreateWindow("Geometry", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                             WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (*window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Window could not be created! SDL_Error: %s", SDL_GetError());
    return ERR_INTERFACE;
  }

  *renderer = SDL_CreateRenderer(
      *window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Renderer could not be created! SDL_Error: %s",
                 SDL_GetError());
    return ERR_INTERFACE;
  }

  ui_reset();
  return OK;
}

err_t draw_scene(SDL_Renderer *renderer) {
  scene_t scene;
  scene.renderer = renderer;
  scene.width = WINDOW_WIDTH;
  scene.height = WINDOW_HEIGHT;
  scene.background = SDL_BLACK;
  scene.foreground = SDL_RED;

  task_t cur_task;
  cur_task.action = DRAW;
  cur_task.scene = scene;

  err_t rc = process_task(cur_task);
  SDL_RenderPresent(renderer);

  return rc;
}

static err_t check_tmp_file() {
  FILE *f = fopen(TEMP_FILE, "r");
  if (f == NULL)
    return ERR_FILE;
  fclose(f);
  return OK;
}

static err_t load_obj(SDL_Renderer *renderer) {
  err_t rc = check_tmp_file();
  if (rc != OK)
    return rc;

  task_t cur_task;
  cur_task.action = LOAD;
  cur_task.filename = TEMP_FILE;

  rc = process_task(cur_task);
  if (rc == OK)
    rc = draw_scene(renderer);
  return rc;
}

static err_t move_obj(SDL_Renderer *renderer) {
  move_t coefs;
  copy_move(coefs, ui.move_coef);

  task_t cur_task;
  cur_task.action = MOVE;
  cur_task.move_coef = coefs;

  err_t rc = process_task(cur_task);

  rc = process_task(cur_task);
  if (rc == OK)
    rc = draw_scene(renderer);
  return rc;
}

static err_t scale_obj(SDL_Renderer *renderer) {
  point_t center;
  copy_point(center, ui.center);

  scale_t coefs;
  copy_scale(coefs, ui.scale_coef);

  task_t cur_task;
  cur_task.action = SCALE;
  cur_task.center = center;
  cur_task.scale_coef = coefs;

  err_t rc = process_task(cur_task);

  rc = process_task(cur_task);
  if (rc == OK)
    rc = draw_scene(renderer);
  return rc;
}

static err_t rotate_obj(SDL_Renderer *renderer) {
  point_t center;
  copy_point(center, ui.center);

  rotate_t coefs;
  copy_rotate(coefs, ui.rotate_coef);

  task_t cur_task;
  cur_task.action = ROTATE;
  cur_task.center = center;
  cur_task.rotate_coef = coefs;

  err_t rc = process_task(cur_task);

  rc = process_task(cur_task);
  if (rc == OK)
    rc = draw_scene(renderer);
  return rc;
}

static err_t get_center(void) {
  char targs[] = {'x', 'y', 'z'};
  double vals[3];
  err_t rc = OK;
  for (size_t i = 0; i < 3 && rc == OK; i++) {
    printf("Введите %c центра поворота/масштабирования: ", targs[i]);
    if (scanf("%lf", &vals[i]) != 1)
      rc = ERR_INPUT;
  }

  if (rc == OK)
    set_point(ui.center, vals[0], vals[1], vals[2]);
  return rc;
}

static err_t get_move_coef(void) {
  char targs[] = {'x', 'y', 'z'};
  double vals[3];
  err_t rc = OK;
  for (size_t i = 0; i < 3 && rc == OK; i++) {
    printf("Введите d%c: ", targs[i]);
    if (scanf("%lf", &vals[i]) != 1)
      rc = ERR_INPUT;
  }
  if (rc == OK)
    set_move(ui.move_coef, vals[0], vals[1], vals[2]);
  return rc;
}

static err_t get_scale_coef(void) {
  char targs[] = {'x', 'y', 'z'};
  double vals[3];
  err_t rc = OK;
  for (size_t i = 0; i < 3 && rc == OK; i++) {
    printf("Введите k%c: ", targs[i]);
    if (scanf("%lf", &vals[i]) != 1)
      rc = ERR_INPUT;
  }
  if (rc == OK)
    set_scale(ui.scale_coef, vals[0], vals[1], vals[2]);
  return rc;
}

static err_t get_rotate_coef(void) {
  char targs[] = {'x', 'y', 'z'};
  double vals[3];
  err_t rc = OK;
  for (size_t i = 0; i < 3 && rc == OK; i++) {
    printf("Введите a%c: ", targs[i]);
    if (scanf("%lf", &vals[i]) != 1)
      rc = ERR_INPUT;
  }
  if (rc == OK)
    set_rotate(ui.rotate_coef, vals[0], vals[1], vals[2]);
  return rc;
}

static err_t handle_move(SDL_Renderer *renderer) {
  err_t rc = get_move_coef();
  if (rc == OK)
    rc = move_obj(renderer);
  return rc;
}

static err_t handle_scale(SDL_Renderer *renderer) {
  err_t rc = get_scale_coef();
  if (rc == OK) {
    rc = get_center();
    if (rc == OK)
      rc = scale_obj(renderer);
  }
  return rc;
}

static err_t handle_rotate(SDL_Renderer *renderer) {
  err_t rc = get_rotate_coef();
  if (rc == OK) {
    rc = get_center();
    if (rc == OK)
      rc = rotate_obj(renderer);
  }
  return rc;
}

static err_t handle_load(SDL_Renderer *renderer) { return load_obj(renderer); }

err_t button_event(SDL_Event *event, SDL_Renderer *renderer) {
  err_t rc = OK;
  switch (event->key.keysym.sym) {
  case SDLK_m:
    rc = handle_move(renderer);
    break;
  case SDLK_s:
    rc = handle_scale(renderer);
    break;
  case SDLK_r:
    rc = handle_rotate(renderer);
    break;
  case SDLK_l:
    rc = handle_load(renderer);
    break;
  default:
    rc = ERR_TASK;
    break;
  }
  return rc;
}

err_t run(SDL_Renderer *renderer) {
  SDL_Event event;
  int quit = 0;
  err_t rc = OK;

  draw_scene(renderer);
  while ((!quit) && (rc == OK)) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT)
        quit = 1;
      else if (event.type == SDL_KEYDOWN)
        rc = button_event(&event, renderer);
    }
    draw_scene(renderer);
  }
  print_err(rc);
  return rc;
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer) {
  task_t cur_task;
  cur_task.action = FREE;
  process_task(cur_task);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
