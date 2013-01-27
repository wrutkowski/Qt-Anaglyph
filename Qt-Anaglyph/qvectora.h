#ifndef QVECTORA_H
#define QVECTORA_H

class QVectorA
{
public:
    double data[4];
    QVectorA();
    void Print ( void );
    void Set ( double d1,double d2,double d3 );
    double GetX();
    double GetY();
    double GetZ();
};

#endif // QVECTORA_H
