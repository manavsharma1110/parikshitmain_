#ifndef MAT_OPS_H
#define MAT_OPS_H
#include "data_structs.h"

MATRIX vec2mat(VECTOR v);
MATRIX mat_mul(MATRIX a, MATRIX b);
MATRIX inverse (MATRIX a);
MATRIX adjoint (MATRIX a);
MATRIX mat_unit(MATRIX a);
MATRIX matrix_vec(QUATERNION a,QUATERNION b,QUATERNION c);
double mat_det(MATRIX a);
MATRIX identity_mat(int a);
MATRIX matzero();
MATRIX cofac(MATRIX a);
MATRIX cons_mul_matrix(MATRIX a, double b);
MATRIX add_mat(MATRIX b,MATRIX a);
MATRIX mat_pow(MATRIX a,int b);
double trace (MATRIX a);
double det(VECTOR a);

MATRIX mat_transpose(MATRIX m);
double determinant (MATRIX a,MATRIX b);
//void display_matrix(MATRIX);

VECTOR mat2vec(MATRIX a);
MATRIX sub_mat(MATRIX a,MATRIX b);

#endif // MAT_OPS_H
