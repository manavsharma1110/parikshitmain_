#ifndef QUAT_OPS_H
#define QUAT_OPS_H
#include "data_structs.h"
QUATERNION angle2quat(double,double,double);
QUATERNION quatmultiply(QUATERNION,QUATERNION);
MATRIX quat2matrix(QUATERNION q);
QUATERNION quatconj(QUATERNION);
QUATERNION quatinv(QUATERNION);
QUATERNION qzero();
double quatsquare(QUATERNION);
double quatnorm(QUATERNION);
VECTOR quat2angle(QUATERNION);
QUATERNION quat_subtract(QUATERNION q1,QUATERNION q2);
QUATERNION quat_add(QUATERNION q1,QUATERNION q2);
QUATERNION scal_mul(QUATERNION,double);
QUATERNION quat_unit(QUATERNION);
double quat_dot(QUATERNION, QUATERNION);
VECTOR quat_rotate(VECTOR,QUATERNION);
#endif // QUAT_OPS_H
