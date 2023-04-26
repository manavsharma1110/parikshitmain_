#include "quat_ops.h"
#include "math.h"
#include "vec_ops.h"
#include <stdio.h>

/*
 * Function: quat2angle
 * --------------------
 *  Calculates an angle vector based on the given quaternion
 *  returns: v (VECTOR)
 */
VECTOR quat2angle(QUATERNION q)
{
    double sqw = q.quat[0]*q.quat[0];
    double sqx = q.quat[1]*q.quat[1];
    double sqy = q.quat[2]*q.quat[2];
    double sqz = q.quat[3]*q.quat[3];
    double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    double test = q.quat[1]*q.quat[2] + q.quat[3]*q.quat[0];
    VECTOR v;
    double PI = 3.1415926;
    if (test > 0.499*unit)   // singularity at north pole
    {
        v.vec[1]= 2 * atan2(q.quat[1],q.quat[0]);
        v.vec[2]= PI/2;
        v.vec[0]= 0;
        return v;
    }
    if (test < -0.499*unit)   // singularity at south pole
    {
        v.vec[1] = -2 * atan2(q.quat[1],q.quat[0]);
        v.vec[2] = -PI/2;
        v.vec[0] = 0;
        return v;
    }
    v.vec[1] = atan2(2*q.quat[1]*q.quat[0]-2*q.quat[1]*q.quat[3] , sqx - sqy - sqz + sqw);
    v.vec[2] = asin(2*test/unit);
    v.vec[0] = atan2(2*q.quat[1]*q.quat[0]-2*q.quat[2]*q.quat[3] , -sqx + sqy - sqz + sqw);
    return v;
}

QUATERNION quat_add(QUATERNION q1,QUATERNION q2)
{
    QUATERNION q0;
    int i;
    for(i=0;i<4;i++)
    {
        q0.quat[i]=q1.quat[i]+q2.quat[i];
    }
    return q0;
}

QUATERNION quat_subtract(QUATERNION q1,QUATERNION q2)
{
    QUATERNION q0;
    int i;
    for(i=0;i<4;i++)
    {
        q0.quat[i]=q1.quat[i]-q2.quat[i];
    }
    return q0;
}

MATRIX quat2matrix(QUATERNION q)
{
    double sqw = q.quat[0]*q.quat[0];
    double sqx = q.quat[1]*q.quat[1];
    double sqy = q.quat[2]*q.quat[2];
    double sqz = q.quat[3]*q.quat[3];

    MATRIX m;
    m.row=3;
    m.col=3;

    // invs (inverse square length) is only required if quaternion is not already normalised
    double invs = 1 / (sqx + sqy + sqz + sqw);
    m.mat[0][0] = ( sqx - sqy - sqz + sqw)*invs ; // since sqw + sqx + sqy + sqz =1/invs*invs
    m.mat[1][1] = (-sqx + sqy - sqz + sqw)*invs ;
    m.mat[2][2] = (-sqx - sqy + sqz + sqw)*invs ;

    double tmp1 = q.quat[1]*q.quat[2];
    double tmp2 = q.quat[3]*q.quat[0];
    m.mat[1][0] = 2.0 * (tmp1 + tmp2)*invs ;
    m.mat[0][1] = 2.0 * (tmp1 - tmp2)*invs ;

    tmp1 = q.quat[1]*q.quat[3];
    tmp2 = q.quat[2]*q.quat[0];
    m.mat[2][0] = 2.0 * (tmp1 - tmp2)*invs ;
    m.mat[0][2] = 2.0 * (tmp1 + tmp2)*invs ;
    tmp1 = q.quat[2]*q.quat[3];
    tmp2 = q.quat[1]*q.quat[0];
    m.mat[2][1] = 2.0 * (tmp1 + tmp2)*invs ;
    m.mat[1][2] = 2.0 * (tmp1 - tmp2)*invs ;

    return m;
}

/*
 * Function: angle2quat
 * --------------------
 *  Calculates a quaternion based on the given euler angles
 *  returns: q (QUATERNION)
 */
QUATERNION angle2quat(double psi,double theta, double phi)
{
    QUATERNION q;
    q.quat[0]=cos(phi/2)*cos(theta/2)*cos(psi/2) + sin(phi/2)*sin(theta/2)*sin(psi/2);
    q.quat[1]=sin(phi/2)*cos(theta/2)*cos(psi/2) - cos(phi/2)*sin(theta/2)*sin(psi/2);
    q.quat[2]=cos(phi/2)*sin(theta/2)*cos(psi/2) + sin(phi/2)*cos(theta/2)*sin(psi/2);
    q.quat[3]=cos(phi/2)*cos(theta/2)*sin(psi/2) - sin(phi/2)*sin(theta/2)*cos(psi/2);
    return q;
}

/*
 * Function: quatmultiply
 * --------------------
 *  Calculates the product of two quaternions.
 *  returns: t (QUATERNION)
 */
QUATERNION quatmultiply(QUATERNION q, QUATERNION r)
{
    QUATERNION t;
    t.quat[0]=t.quat[1]=t.quat[2]=t.quat[3]=0;
    t.quat[0]=r.quat[0]*q.quat[0] - r.quat[1]*q.quat[1] - r.quat[2]*q.quat[2] - r.quat[3]*q.quat[3];
    t.quat[1]=r.quat[0]*q.quat[1] + r.quat[1]*q.quat[0] - r.quat[2]*q.quat[3] + r.quat[3]*q.quat[2];
    t.quat[2]=r.quat[0]*q.quat[2] + r.quat[1]*q.quat[3] + r.quat[2]*q.quat[0] - r.quat[3]*q.quat[1];
    t.quat[3]=r.quat[0]*q.quat[3] - r.quat[1]*q.quat[2] + r.quat[2]*q.quat[1] + r.quat[3]*q.quat[0];
    return t;
}

//VECTOR quat_rotate(VECTOR v, QUATERNION q)
//{

  //  return;
//}
/*
 * Function: quatmultiply
 * --------------------
 *  Calculates the conjugate of a quaternion which is [w -a -b -c]  if [w a b c] is the original quaternion
 *  returns: qconj (QUATERNION)
 */
QUATERNION quatconj(QUATERNION q)
{
    int i;
    QUATERNION qconj;
    qconj.quat[0]=q.quat[0];
    for(i=1; i<4; i++)
        qconj.quat[i]=q.quat[i]*-1;
    return qconj;
}

/*
 * Function: quatmultiply
 * --------------------
 *  Calculates the inverse of a quaternion [w a b c]
 *  returns: qinv (QUATERNION)
 */
QUATERNION quatinv(QUATERNION q)
{
    double sq;
    int i;
    QUATERNION qinv;
    sq=quatsquare(q);
    qinv=quatconj(q);
    for(i=0; i<4; i++)
        qinv.quat[i]=qinv.quat[i]/sq;
    return qinv;
}

/*
 * Function: qzero
 * --------------------
 *  Makes all elements of quat array in QUATERNION struct as zero.
 *  returns: q (QUATERNION)
 */
QUATERNION qzero()
{
    QUATERNION q;
    int i;
    q.quat[0]=1;
    for(i=1; i<4; i++) q.quat[i]=0;
    return q;
}

/*
 * Function: quatsquare
 * --------------------
 *  Calculates the square of a given quaternion
 *  returns: sq (double)
 */
double quatsquare(QUATERNION q)
{
    int i;
    double sq=0;
    for(i=0; i<4; i++) sq = sq+ q.quat[i]*q.quat[i];
    return sq;
}

/*
 * Function: quatnorm
 * --------------------
 *  Magnitude of the quaternion q
 *  returns: (double)
 */
double quatnorm(QUATERNION q)
{
    return sqrt(quatsquare(q));
}



QUATERNION scal_mul(QUATERNION q,double f)
{
    int i;
    for(i=0;i<4;i++)
        q.quat[i]=q.quat[i]*f;
    return q;
}

QUATERNION quat_unit(QUATERNION q)
{
    int i;
    for(i=0;i<4;i++)
        q.quat[i]=q.quat[i]/quatnorm(q);
    return q;
}
double quat_dot(QUATERNION u, QUATERNION v)
{
    double x;
    x=v.quat[0]*u.quat[0] + v.quat[1]*u.quat[1] + v.quat[2]*u.quat[2] + v.quat[3]*u.quat[3];
    return x;
}
