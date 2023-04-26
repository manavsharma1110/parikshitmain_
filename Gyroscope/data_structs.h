#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

struct matrix
{
    double mat[10][10];
    int row,col;
};
struct Vector
{
    double vec[3];
};
struct quaternion
{
    double quat[4];
};

struct dynamics_return_block
{
    struct Vector w_bi;
    struct Vector Tc;
    struct quaternion q_dot;
    struct quaternion q_ref;
};

struct op_return_block
{
    struct matrix mag_orb;
    struct Vector v;
    double vnew[3];
    double rnew[3];
};
typedef struct matrix MATRIX;
typedef struct Vector VECTOR;
typedef struct quaternion QUATERNION;
typedef struct dynamics_return_block DYNAMICS;
typedef struct op_return_block OP_ELEMENTS;
struct quest_par
{
    MATRIX mat;
    MATRIX m1;
};
typedef struct quest_par QUEST_PAR;
#endif // DATA_STRUCTS_H
