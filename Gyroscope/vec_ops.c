#include "vec_ops.h"
#include "math.h"
#include <stdio.h>
VECTOR cross_product(VECTOR r,VECTOR v)
{
    VECTOR h;
    h.vec[0]= r.vec[1]*v.vec[2]-r.vec[2]*v.vec[1];
    h.vec[1]= r.vec[2] * v.vec[0] - r.vec[0]*v.vec[2];
    h.vec[2]= r.vec[0]*v.vec[1] - r.vec[1]*v.vec[0];
    return h;
}

VECTOR vec_add(VECTOR a,VECTOR b){
    VECTOR v;
    int i;
    for(i=0;i<3;i++){
        v.vec[i]=a.vec[i]+b.vec[i];
    }
    return v;
}

VECTOR vec_sub(VECTOR a,VECTOR b){
    VECTOR v;
    int i;
    for(i=0;i<3;i++){
        v.vec[i]=a.vec[i]-b.vec[i];
    }
    return v;
}

VECTOR vec_pow(VECTOR v, int x)
{
    int i;
    for(i=0;i<4;i++)
        v.vec[i]=pow(v.vec[i],x);
    return v;
}

VECTOR vec_unit(VECTOR x)
{
    double y=0;
    int i;
    for(i=0;i<3;i++)
        y=y+pow(x.vec[i],2);
    y=sqrt(y);
    for(i=0;i<3;i++)
        x.vec[i]=x.vec[i]/y;
    return x;
}

VECTOR veczero()
{
    VECTOR v;
    int i;
    for(i=0; i<3; i++) v.vec[i]=0;
    return v;
}

double vec_mag(VECTOR v)
{
    return (sqrt(v.vec[0]*v.vec[0]+v.vec[1]*v.vec[1]+v.vec[2]*v.vec[2]));
}

QUATERNION vect2quat(VECTOR v)
{
    QUATERNION q;
    q.quat[0]=0;
    q.quat[1]=v.vec[0];
    q.quat[2]=v.vec[1];
    q.quat[3]=v.vec[2];
    return q;
}

MATRIX vec2mat_col (VECTOR v)
{
    MATRIX m;
    m.row=m.col=3;
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            m.mat[i][j]=0;
    m.mat[0][0]=v.vec[0];
    m.mat[1][0]=v.vec[1];
    m.mat[2][0]=v.vec[2];
    return m;
}

MATRIX vec2mat_row(VECTOR v)
{
    MATRIX m;
    m.row=m.col=3;
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            m.mat[i][j]=0;
    m.mat[0][0]=v.vec[0];
    m.mat[0][1]=v.vec[1];
    m.mat[0][2]=v.vec[2];
    return m;
}

double dot_product(VECTOR v,VECTOR u){
    double x;
    x=v.vec[0]*u.vec[0] + v.vec[1]*u.vec[1] + v.vec[2]*u.vec[2];
    return x;
}

VECTOR vec_multiply(double y,VECTOR x)
{
    int i;
    for(i=0;i<3;i++)
        x.vec[i]=x.vec[i]*y;
    return x;
}

void display_vector(VECTOR v)
{
    int i;
    for(i=0; i<3; i++)
        printf("%0.20lf\t",v.vec[i]);
}
