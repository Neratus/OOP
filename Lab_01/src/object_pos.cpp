#include "object_pos.h"

void reset_move(move_t &coefs) {
  coefs.dx = 0;
  coefs.dy = 0;
  coefs.dz = 0;
}

void reset_rotate(rotate_t &coefs) {
  coefs.ax = 0;
  coefs.ay = 0;
  coefs.az = 0;
}

void reset_scale(scale_t &coefs) {
  coefs.kx = 0;
  coefs.ky = 0;
  coefs.kz = 0;
}

void set_move(move_t &coefs, double &dx, double &dy, double &dz) {
  coefs.dx = dx;
  coefs.dy = dy;
  coefs.dz = dz;
}

void set_rotate(rotate_t &coefs, double &ax, double &ay, double &az) {
  coefs.ax = ax;
  coefs.ay = ay;
  coefs.az = az;
}

void set_scale(scale_t &coefs, double &kx, double &ky, double &kz) {
  coefs.kx = kx;
  coefs.ky = ky;
  coefs.kz = kz;
}

void copy_move(move_t &dest, move_t &src) {
  dest.dx = src.dx;
  dest.dy = src.dy;
  dest.dz = src.dz;
}

void copy_rotate(rotate_t &dest, rotate_t &src) {
  dest.ax = src.ax;
  dest.ay = src.ax;
  dest.az = src.ax;
}

void copy_scale(scale_t &dest, scale_t &src) {
  dest.kx = src.kx;
  dest.ky = src.kx;
  dest.kz = src.kx;
}
