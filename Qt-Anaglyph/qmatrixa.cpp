#include "qmatrixa.h"

#include <QDebug>
#include <math.h>
#include "qmatrixa.h"


QMatrixA::QMatrixA()
{
    data[0][0]=0.0; data[0][1]=0.0; data[0][2]=0.0; data[0][3]=0.0;
    data[1][0]=0.0; data[1][1]=0.0; data[1][2]=0.0; data[1][3]=0.0;
    data[2][0]=0.0; data[2][1]=0.0; data[2][2]=0.0; data[2][3]=0.0;
    data[3][0]=0.0; data[3][1]=0.0; data[3][2]=0.0; data[3][3]=1.0;
}

void QMatrixA::Print ( void )
{
    qDebug() << QString ( "\n|%1, %2, %3, %4|")  .arg(data[0][0]).arg(data[0][1]).arg(data[0][2]).arg(data[0][3]);
    qDebug() << QString ( "|%1, %2, %3, %4|")    .arg(data[1][0]).arg(data[1][1]).arg(data[1][2]).arg(data[1][3]);
    qDebug() << QString ( "|%1, %2, %3, %4|")    .arg(data[2][0]).arg(data[2][1]).arg(data[2][2]).arg(data[2][3]);
    qDebug() << QString ( "|%1, %2, %3, %4|")    .arg(data[3][0]).arg(data[3][1]).arg(data[3][2]).arg(data[2][3]);
}

QMatrixA QMatrixA::operator* ( const QMatrixA gMatrix )
{
    int i,j,k;
    QMatrixA tmp;

    for ( i=0; i<4; i++ )
        for ( j=0; j<4; j++ )
        {
            tmp.data[i][j]=0.0;
            for ( k=0; k<4; k++ )
                tmp.data[i][j]=tmp.data[i][j]+ ( data[i][k]*gMatrix.data[k][j] );
        }
    return tmp;
}

QVectorA operator* ( const QMatrixA gMatrix,const QVectorA gVector )
{
    unsigned int i,j;
    QVectorA tmp;

    for ( i=0; i<4; i++ )
    {
        tmp.data[i]=0.0;
        for ( j=0; j<4; j++ ) tmp.data[i]=tmp.data[i]+ ( gMatrix.data[i][j]*gVector.data[j] );
    }

    for(int i = 0; i < 4; ++i)
        tmp.data[i] /= tmp.data[3];

    return tmp;
}

QMatrixA QMatrixA::translation(double DeltaX, double DeltaY, double DeltaZ)
{
    QMatrixA t;

    t.data[0][0]=1.0;                                           t.data[0][3]=DeltaX;
                        t.data[1][1]=1.0;                       t.data[1][3]=DeltaY;
                                            t.data[2][2]=1.0;   t.data[2][3]=DeltaZ;

    return t;
}

QMatrixA QMatrixA::scale(double SX, double SY, double SZ)
{
    QMatrixA t;

    t.data[0][0]=SX;
                        t.data[1][1]=SY;
                                            t.data[2][2]=SZ;

    return t;
}

QMatrixA QMatrixA::rotateOX(double ox)
{
    ox *= degToRad;

    QMatrixA tx;

    tx.data[0][0]=1.0;
                            tx.data[1][1]= cos(ox);     tx.data[1][2]=-sin(ox);
                            tx.data[2][1]= sin(ox);     tx.data[2][2]= cos(ox);
    return tx;
}

QMatrixA QMatrixA::rotateOY(double oy)
{
    oy *= degToRad;

    QMatrixA ty;

    ty.data[0][0]= cos(oy);                             ty.data[0][2]= sin(oy);
                            ty.data[1][1]=1.0;
    ty.data[2][0]=-sin(oy); ty.data[2][1]=1.0;          ty.data[2][2]= cos(oy);

    return ty;
}
QMatrixA QMatrixA::rotateOZ(double oz)
{
    oz *= degToRad;

    QMatrixA tz;

    tz.data[0][0]= cos(oz); tz.data[0][1]=-sin(oz);
    tz.data[1][0]= sin(oz); tz.data[1][1]= cos(oz);
                                                        tz.data[2][2]=1.0;

    return tz;
}


QMatrixA QMatrixA::rotateEuler(double phi, double psi, double theta)
{
    static const double degToRad = M_PI / 180;
    phi   *= degToRad;
    psi   *= degToRad;
    theta *= degToRad;

    QMatrixA t;

    t.data[0][0]= cos(phi)*cos(theta) - sin(phi)*cos(psi)*sin(theta);   t.data[0][1]= sin(phi)*cos(theta) + cos(phi)*cos(psi)*sin(theta);   t.data[0][2]=sin(psi)*sin(theta);
    t.data[1][0]=-cos(phi)*cos(theta) - sin(phi)*cos(psi)*cos(theta);   t.data[1][1]=-sin(phi)*sin(theta) + cos(phi)*cos(psi)*cos(theta);   t.data[1][2]=sin(psi)*cos(theta);
    t.data[2][0]= sin(phi)*sin(psi);                                    t.data[2][1]= cos(phi)*sin(psi);                                    t.data[2][2]=cos(phi);

    return t;
}



QMatrixA QMatrixA::rotate(double ox, double oy, double oz)
{
    static const double degToRad = M_PI / 180;
    ox *= degToRad;
    oy *= degToRad;
    oz *= degToRad;

    QMatrixA tx;

    tx.data[0][0]=1.0;
                            tx.data[1][1]= cos(ox);     tx.data[1][2]=-sin(ox);
                            tx.data[2][1]= sin(ox);     tx.data[2][2]= cos(ox);

    QMatrixA ty;

    ty.data[0][0]= cos(oy);                             ty.data[0][2]= sin(oy);
                            ty.data[1][1]=1.0;
    ty.data[2][0]=-sin(oy);                             ty.data[2][2]= cos(oy);

    QMatrixA tz;

    tz.data[0][0]= cos(oz); tz.data[0][1]=-sin(oz);
    tz.data[1][0]= sin(oz); tz.data[1][1]= cos(oz);
                                                        tz.data[2][2]=1.0;

    return tx * ty * tz;
}

QMatrixA QMatrixA::worldToScreen(double x_0, double x_1, double y_0, double y_1, double x_min, double x_max, double y_min, double y_max )
{
    int k, j;
    //div by 0!
    if(x_1 == x_0) k = 1; else k = (x_1 - x_0);
    if(y_1 == y_0) k = 1; else k = (y_1 - y_0);

    double S_x = ( x_max - x_min ) / k;
    double S_y = ( y_max - y_min ) / k;

    QMatrixA t;

    t.data[0][0]=double(S_x);                                           t.data[0][3]= double(x_min - S_x * x_0);
                        t.data[1][1]=double(S_y);                       t.data[1][3]= double(y_min - S_y * y_0);

    return t;
}


void QMatrixA::line2d( QMatrixA t, QVectorA * v1, QVectorA * v2 )
{
    QVectorA new_v1 = t * *v1;
    QVectorA new_v2 = t * *v2;

    for(int i = 0; i < 4; ++i)
        v1->data[i] = new_v1.data[i];

    for(int i = 0; i < 4; ++i)
        v2->data[i] = new_v2.data[i];
}

QMatrixA QMatrixA::perspectiveView(double d)
{
    QMatrixA t;

    t.data[0][0]=1.0;
                        t.data[1][1]=1.0;
                                            //0.0
                                            t.data[3][2]=1.0/d; //1.0

    return t;
}
