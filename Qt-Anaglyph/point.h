#ifndef POINT_H
#define POINT_H

struct Point2D
{
    double x;
    double y;
};

struct Point3D
    : public Point2D
{
    double z;
};

#endif // POINT_H
