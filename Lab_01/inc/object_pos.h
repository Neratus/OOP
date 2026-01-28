#ifndef OBJ_POS_H
#define OBJ_POS_H

struct move_t {
  double dx;
  double dy;
  double dz;
};

struct scale_t {
  double kx;
  double ky;
  double kz;
};

struct rotate_t {
  double ax;
  double ay;
  double az;
};

void reset_move(move_t &coefs);

void reset_rotate(rotate_t &coefs);

void reset_scale(scale_t &coefs);

void set_move(move_t &coefs, double &dx, double &dy, double &dz);

void set_rotate(rotate_t &coefs, double &ax, double &ay, double &az);

void set_scale(scale_t &coefs, double &kx, double &ky, double &kz);

void copy_move(move_t &dest, move_t &src);

void copy_rotate(rotate_t &dest, rotate_t &src);

void copy_scale(scale_t &dest, scale_t &src);

#endif
