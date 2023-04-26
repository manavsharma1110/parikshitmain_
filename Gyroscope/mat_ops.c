#include "mat_ops.h"
#include "math.h"
#include "vec_ops.h"
#include "quat_ops.h"
#include <stdio.h>
#include "cpal.h"

extern CPAL_InitTypeDef I2C2_DevStructure;
MATRIX matrix_vec(QUATERNION a, QUATERNION b, QUATERNION c)
{
    int i,j;
    MATRIX m;
    for(i=0;i<4;i++)
    {
        for(j=0;j<3;j++)
        {
            if(j==0) m.mat[i][j]=a.quat[i];
            else if(j==1) m.mat[i][j]=b.quat[i];
            else if(j==2) m.mat[i][j]=c.quat[i];
       }
    }
    m.row=4; m.col=3;
    return m;
}

MATRIX sum;
MATRIX mat_mul(MATRIX a, MATRIX b)
{


sum.row=a.row;
sum.col=b.col;
    int i,j,k;
    for(i=0; i<sum.row; i++)
    {
        for(j=0; j<sum.col; j++)
        {
            sum.mat[i][j]=0;
            for(k=0; k<a.col; k++)
            {
                sum.mat[i][j]+=a.mat[i][k]*b.mat[k][j];
            }
        }
    }
    return sum;
}

MATRIX adjoint(MATRIX I)
{
    MATRIX Idash;
    Idash.row=Idash.col=3;
    /*Idash.mat[0][0]=(I.mat[1][1]) *(I.mat[2][2]) - (I.mat[1][2]) * (I.mat[2][1]);
    Idash.mat[0][1]=(I.mat[1][2]) *(I.mat[2][0]) - (I.mat[1][0]) * (I.mat[2][2]);
    Idash.mat[2][0]=(I.mat[1][0]) *(I.mat[2][1]) - (I.mat[1][1]) * (I.mat[2][0]);
    Idash.mat[1][0]=(I.mat[2][1]) *(I.mat[0][2]) - (I.mat[2][2]) * (I.mat[0][1]);
    Idash.mat[1][1]=(I.mat[2][2]) *(I.mat[0][0]) - (I.mat[2][0]) * (I.mat[0][2]);
    Idash.mat[1][2]=(I.mat[2][0]) *(I.mat[0][1]) - (I.mat[2][1]) * (I.mat[0][0]);
    Idash.mat[0][2]=(I.mat[0][1]) *(I.mat[1][2]) - (I.mat[0][2]) * (I.mat[1][1]);
    Idash.mat[2][1]=(I.mat[0][2]) *(I.mat[1][0]) - (I.mat[0][0]) * (I.mat[1][2]);
    Idash.mat[2][2]=(I.mat[0][0]) *(I.mat[1][1]) - (I.mat[0][1]) * (I.mat[1][0]);
    */
    Idash.mat[0][0]=(I.mat[1][1]) *(I.mat[2][2]) - (I.mat[1][2]) * (I.mat[2][1]);
    Idash.mat[0][1]=(I.mat[0][2]) *(I.mat[2][1]) - (I.mat[2][2]) * (I.mat[0][1]);
    Idash.mat[2][0]=(I.mat[1][0]) *(I.mat[2][1]) - (I.mat[1][1]) * (I.mat[2][0]);
    Idash.mat[1][0]=(I.mat[2][0]) *(I.mat[1][2]) - (I.mat[2][2]) * (I.mat[1][0]);
    Idash.mat[1][1]=(I.mat[2][2]) *(I.mat[0][0]) - (I.mat[2][0]) * (I.mat[0][2]);
    Idash.mat[1][2]=(I.mat[0][2]) *(I.mat[1][0]) - (I.mat[1][2]) * (I.mat[0][0]);
    Idash.mat[0][2]=(I.mat[0][1]) *(I.mat[1][2]) - (I.mat[1][1]) * (I.mat[0][2]);
    Idash.mat[2][1]=(I.mat[2][0]) *(I.mat[0][1]) - (I.mat[0][0]) * (I.mat[2][1]);
    Idash.mat[2][2]=(I.mat[0][0]) *(I.mat[1][1]) - (I.mat[0][1]) * (I.mat[1][0]);

    return Idash;
}

double determinant(MATRIX a,MATRIX Idash)
{
    double Imod=0;
    Imod=Imod + Idash.mat[0][0]*a.mat[0][0] + Idash.mat[1][0]*a.mat[0][1] + Idash.mat[2][0]*a.mat[0][2];
    return Imod;
}

MATRIX inverse(MATRIX m)
{
    int i,j;
   MATRIX Idash1;
   
   Idash1.mat[0][0] = 1;
   Idash1.mat[0][1] = 2;
   
    MATRIX Idash =adjoint(m);
    
   Idash.row=Idash.col=3;
   double imod=determinant(m,Idash);
   
   for(i=0; i<3; i++)
     for(j=0; j<3; j++)
       Idash.mat[i][j]=Idash.mat[i][j] /imod;
    return Idash1;
}

MATRIX matzero()
{
    MATRIX m;
    int i,j;
    m.row=m.col=3;
    for(i=0; i<10; i++)
        for(j=0; j<10; j++)
            m.mat[i][j]=0;
    return m;
}


MATRIX cofac(MATRIX m)
{
    int i,j,a,b,ct=0;
    MATRIX mm;
    mm.row=m.row;
    mm.col=m.col;
    double val[4];
    for(i=0;i<m.row;i++)
    {
        for(j=0;j<m.col;j++)
        {
            for(a=0;a<m.row;a++)
            {
                for(b=0;b<m.col;b++)
                {
                    if(a!=i && b!=j)
                    {
                        val[ct]=m.mat[a][b];
                        ct++;
                    }
                }
            }
            mm.mat[i][j]=val[0]*val[3]-val[1]*val[2];
            mm.mat[i][j]=mm.mat[i][j]*pow((i+j),-1);
        }
    }
    return mm;
}


double trace (MATRIX m)
{
    int i;
    double Sum=0;
    for(i=0;i<m.row;i++)
        Sum+=m.mat[i][i];
        //Sum=m.mat[0][0]+m.mat[1][1]+m.mat[2][2];
    return Sum;
}

MATRIX mat_transpose(MATRIX m)
{
    int i,j;
    MATRIX m2;
    m2.row=m.col;
    m2.col=m.row;
    for(i=0;i<m2.row;i++)
    {
        for(j=0;j<m2.col;j++)
        {
            m2.mat[i][j]=m.mat[j][i];
        }
    }
 //   display_matrix(m2);
    return m2;
}

MATRIX mat_pow(MATRIX m,int a)
{
    int i,j;
    for(i=0;i<m.row;i++)
        for(j=0;j<m.col;j++)
            m.mat[i][j]=pow(m.mat[i][j],a);
    return m;
}

MATRIX identity_mat(int x)
{
    MATRIX m;
    m.row=m.col=x;
    int i,j;
    for(i=0;i<m.row;i++)
        for(j=0;j<m.col;j++)
            if(i==j)m.mat[i][j]=1;
            else m.mat[i][j]=0;
    return m;

}

MATRIX cons_mul_matrix(MATRIX m,double x)
{
    int i,j;
    for(i=0;i<m.row;i++)
        for(j=0;j<m.col;j++)
            m.mat[i][j]=m.mat[i][j]*x;
    return m;
}

MATRIX add_mat(MATRIX a,MATRIX b)
{
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            a.mat[i][j]=a.mat[i][j]+b.mat[i][j];
    return a;
}

MATRIX sub_mat(MATRIX a,MATRIX b)
{
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            a.mat[i][j]=a.mat[i][j]-b.mat[i][j];
    return a;
}



//MATRIX backslash(MATRIX A, MATRIX C)
//{
//    QUATERNION a1= {A.mat[0][0],A.mat[1][0],A.mat[2][0],A.mat[3][0]};
//    QUATERNION a2= {A.mat[0][1],A.mat[1][1],A.mat[2][1],A.mat[3][1]};
//    QUATERNION a3= {A.mat[0][2],A.mat[1][2],A.mat[2][2],A.mat[3][2]};
//    QUATERNION e1= quat_unit(a1);
//    QUATERNION u2= quat_subtract(a2,scal_mul(e1,quat_dot(e1,a2)));
//    QUATERNION e2= quat_unit(u2);
//    QUATERNION u3= quat_subtract(quat_subtract(a3,scal_mul(e1,quat_dot(a3,e1))),scal_mul(e2,quat_dot(a3,e2)));
//    QUATERNION e3= quat_unit(u3);
//    MATRIX Q= matrix_vec(e1,e2,e3);
//    MATRIX R=mat_mul(mat_transpose(Q),A);
//    MATRIX Cq=mat_mul(inverse(R),mat_transpose(Q));
//    MATRIX x=mat_mul(Cq,C);
//    return x;
//}

VECTOR mat2vec(MATRIX m){
    VECTOR v;
    int i;
    for(i=0;i<m.row;i++){
        v.vec[i]=m.mat[i][0];
    }
    return v;
}
MATRIX vec2mat(VECTOR v){
    MATRIX m;
    m.row=3;
    m.col=1;
    int i;
    for(i=0;i<3;i++){
        m.mat[i][0]=v.vec[i];
    }
    return m;
}
