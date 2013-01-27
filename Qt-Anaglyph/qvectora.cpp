#include "qvectora.h"
#include <QDebug>

QVectorA::QVectorA()
{
    data[0]=0.0;
    data[1]=0.0;
    data[2]=0.0;
    data[3]=1.0;
}

void QVectorA::Print ( void )
{
    qDebug() <<  QString( "(%1,%2,%3,%4)\n" ).arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]);
}

void QVectorA::Set ( double d1,double d2,double d3 )
{
    data[0]=d1;
    data[1]=d2;
    data[2]=d3;
}

double QVectorA::GetX()
{
    return data[0];
}

double QVectorA::GetY()
{
    return data[1];
}

double QVectorA::GetZ()
{
    return data[2];
}
