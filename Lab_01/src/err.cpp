#include "stdio.h"

#include "err.h"

void print_err(const err_t error) {
  switch (error) {
  case OK:
    break;
  case ERR_INPUT:
    printf("Ошибка ввода \n");
    break;
  case ERR_SIZE:
    printf("Ошибка некорректного значения размеров \n");
    break;
  case ERR_MALLOC:
    printf("Ошибка выделения памяти \n");
    break;
  case ERR_PARAM:
    printf("Ошибка: в функцию переданы некорретные параметры \n");
    break;
  case ERR_FILE:
    printf("Ошибка: не удалось открыть файл \n");
    break;
  case ERR_INTERFACE:
    printf("Ошибка: не удалось отобразить интерфейс \n");
    break;
  case ERR_TASK:
    printf("Ошибка: некорретная опция действия \n");
    break;
  default:
    printf("Unknown error");
    break;
  }
}
