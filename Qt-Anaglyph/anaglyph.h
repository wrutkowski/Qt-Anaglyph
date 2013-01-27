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
    void setCoordDist(double dist) {_coordinateDistance = dist;}
    void setObservDist(double dist) {_coordinateDistance = dist;}
    double getCoordDist() const {return _coordinateDistance;}
    double getObservDist() const {return _observerDistance;}
    Point2D leftEyeView(Point3D point);
    Point2D rightEyeView(Point3D point);
    void generate();
private:
    double _coordinateDistance; //odleglosc srodka ukladu od ekranu (przydalby sie do tego suwak)
    double _observerDistance; //odleglosc oczu od monitora
    const double _eyeDistance; //rozstaw oczu, tu przydaloby sie przetestowac bo nie wiem jak to przeliczyc na jednoski ukladu

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
