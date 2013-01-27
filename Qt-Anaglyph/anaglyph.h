#ifndef ANAGLYPH_H
#define ANAGLYPH_H

#include <QImage>
#include <QColor>
#include <QList>
#include <vector>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aline.h"
#include "point.h"

class Anaglyph
{
public:
    Anaglyph();
    void setData(QStringList data);
    QList<ALine> getData() const { return _data; }
    QImage getGeneratedAnaglyph() const { return _anaglyphImage; }
    void setGlassesColors(QColor colorLeftEye, QColor colorRightEye) { _colorLeftEye = colorLeftEye; _colorRightEye = colorRightEye; }
    void setBackgroundColor(QColor backgroundColor) { _backgroundColor = backgroundColor; }
    QColor getBackgroundColor() const { return _backgroundColor; }
    void setAxis(int X, int Y, int Z) { _X = X; _Y = Y; _Z = Z; }
    void setCoordDist(double dist) {_coordinateDistance = dist;}
    void setObservDist(double dist) {_coordinateDistance = dist;}
    void setEyeDist(double dist) {_eyeDistance = dist;}
    double getEyeDist() const {return _eyeDistance;}
    double getCoordDist() const {return _coordinateDistance;}
    double getObservDist() const {return _observerDistance;}
    Point2D leftEyeView(double x, double y, double z);
    Point2D rightEyeView(double x, double y, double z);
    void generate(Ui::MainWindow *ui);
private:
    double _coordinateDistance; // distance between (0,0,0) and the screen; slider will be good
    double _observerDistance; // distance between eyes and the screen
    double _eyeDistance; // distance between eyes; test - find the correlacion between mm and units


    QList<ALine> _data; // list of all lines of the shape

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
