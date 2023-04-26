#ifndef VEC_OPS_H
#define VEC_OPS_H
#include "data_structs.h"
VECTOR cross_product(VECTOR,VECTOR);
VECTOR veczero();
QUATERNION vect2quat(VECTOR);
double vec_mag(VECTOR);
void display_vector(VECTOR);
VECTOR vec_add(VECTOR a,VECTOR b);
VECTOR vec_sub(VECTOR a,VECTOR b);
MATRIX vec2mat_col(VECTOR);
MATRIX vec2mat_row(VECTOR);
double dot_product(VECTOR,VECTOR);
VECTOR vec_pow(VECTOR,int);
MATRIX vec_transpose(VECTOR);
VECTOR vec_multiply(double,VECTOR);
VECTOR vec_unit(VECTOR);

#endif // VEC_OPS_H
