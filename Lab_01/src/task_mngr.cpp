#include "task_mngr.h"

err_t process_task(task_t &task) {
  err_t rc = OK;
  static object_t obj = object_init();

  switch (task.action) {
  case MOVE:
    rc = object_move(obj, task.move_coef);
    break;
  case SCALE:
    rc = object_scale(obj, task.center, task.scale_coef);
    break;
  case ROTATE:
    rc = object_rotate(obj, task.center, task.rotate_coef);
    break;
  case LOAD:
    rc = object_load(obj, task.filename);
    break;
  case DRAW:
    rc = object_draw(obj, task.scene);
    break;
  case FREE:
    object_free(obj);
    break;
  default:
    rc = ERR_TASK;
    break;
  }

  return rc;
}
