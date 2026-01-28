#ifndef ERR_H
#define ERR_H

#include <cstdio>

enum err_t {
  OK,
  ERR_INPUT,
  ERR_SIZE,
  ERR_MALLOC,
  ERR_PARAM,
  ERR_FILE,
  ERR_INTERFACE,
  ERR_TASK
};

void print_err(const err_t error);

#endif
