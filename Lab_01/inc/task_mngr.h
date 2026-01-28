#ifndef TASK_MNGR
#define TASK_MNGR

#include "draw.h"
#include "err.h"
#include "object.h"

enum action_t { LOAD, MOVE, SCALE, ROTATE, DRAW, FREE };

struct task_t {
  action_t action;
  point_t center;
  union {
    const char *filename;
    move_t move_coef;
    scale_t scale_coef;
    rotate_t rotate_coef;
    scene_t scene;
  };
};

err_t process_task(task_t &task);

#endif
