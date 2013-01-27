#ifndef ANAGLYPH_H
#define ANAGLYPH_H

#include <QImage>
#include <QColor>
#include <QList>
#include <vector>
#include "AObject.h"
#include "ALine.h"
#include "point.h"

class Anaglyph
{
public:
    Anaglyph();
    void setData(QStringList data);
    QImage getGeneratedAnaglyph() const { return _anaglyphImage; }
    void setGlassesColors(QColor colorLeftEye, QColor colorRightEye) { _colorLeftEye = colorLeftEye; _colorRightEye = colorRightEye; }
    void setBackgroundColor(QColor backgroundColor) { _backgroundColor = backgroundColor; }
    void setAxis(int X, int Y, int Z) { _X = X; _Y = Y; _Z = Z; }
    Point2D leftEyeView(Point3D point);
    Point2D rightEyeView(Point3D point);
    void generate();
private:
    double _coordinateDistance; //odleglosc srodka ukladu od ekranu
    double _observerDistance; //odleglosc oczu od monitora
    double _eyeDistance; //rozstaw oczu

    QList<ALine> _data;

    int _X;
    int _Y;
    int _Z;

    int _glassesType;
    QColor _backgroundColor;
    QColor _colorLeftEye;
    QColor _colorRightEye;

    QImage _anaglyphImage;

};

#endif // ANAGLYPH_H
