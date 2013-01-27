#ifndef QMATRIXA_H
#define QMATRIXA_H

#include "qvectora.h"

class QMatrixA
{
public:
    static const double degToRad = 3.14161 / 180;
    double data[4][4];
    QMatrixA();
    void Print ( void );
    QMatrixA operator* ( const QMatrixA );
    friend QVectorA operator* ( const QMatrixA,const QVectorA );
    static QMatrixA translation(double DeltaX, double DeltaY, double DeltaZ);
    static QMatrixA scale(double SX, double SY, double SZ);
    static QMatrixA worldToScreen(double x_0, double x_1, double y_0, double y_1, double x_min, double x_max, double y_min, double y_max );
    static QMatrixA rotateOX(double alpha);
    static QMatrixA rotateOY(double alpha);
    static QMatrixA rotateOZ(double alpha);
    static QMatrixA rotate(double ox, double oy, double oz);
    static QMatrixA rotateEuler(double phi, double psi, double theta);
    static QMatrixA perspectiveView(double d);
    static void line2d( QMatrixA t, QVectorA * v1, QVectorA * v2 );
};

#endif // QMATRIXA_H
