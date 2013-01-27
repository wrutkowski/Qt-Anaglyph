#ifndef ALINE_H
#define ALINE_H

#include "AObject.h"

class ALine : AObject {
public:
    ALine(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2) { x1 = _x1; y1 = _y1; z1 = _z1; x2 = _x2; y2 = _y2; z2 = _z2; }

    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;

private:

};

#endif // ALINE_H
