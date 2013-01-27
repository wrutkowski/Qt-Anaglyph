#ifndef ANAGLYPH_H
#define ANAGLYPH_H

#include <QImage>
#include <QColor>
#include <QList>
#include "AObject.h"
#include "ALine.h"

#define kAnaglyphGlassesRedCyan     0
#define kAnaglyphGlassesRedBlue     1
#define kAnaglyphGlassesRedGreen    2

class Anaglyph
{
public:
    Anaglyph();
    void setData(QStringList data);
    QImage getGeneratedAnaglyph() const { return _anaglyphImage; }
    void setGlassesType(int glassesType) { _glassesType = glassesType; }
    void setBackgroundColor(QColor backgroundColor) { _backgroundColor = backgroundColor; }
    void setAxis(int X, int Y, int Z) { _X = X; _Y = Y; _Z = Z; }

    void generate();
private:
    QList<ALine> _data;

    int _X;
    int _Y;
    int _Z;

    int _glassesType;
    QColor _backgroundColor;

    QImage _anaglyphImage;

};

#endif // ANAGLYPH_H
