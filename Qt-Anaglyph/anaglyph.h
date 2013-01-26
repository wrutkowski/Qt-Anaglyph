#ifndef ANAGLYPH_H
#define ANAGLYPH_H

#include <QImage>
#include <QColor>

#define kAnaglyphGlassesRedCyan     0
#define kAnaglyphGlassesRedBlue     1
#define kAnaglyphGlassesRedGreen    2

#define kAnaglyphOptionCrop         0
#define kAnaglyphOptionBackground   1

class Anaglyph
{
public:
    Anaglyph();
    bool addLeftEyeImage(const QString& fileName, const char* format=0) {return _leftEyeImage.load(fileName, format);}
    bool addRightEyeImage(const QString& fileName, const char* format=0) {return _rightEyeImage.load(fileName, format);}
    QImage getRigthEyeImage() const {return _rightEyeImage;}
    QImage getLeftEyeImage() const {return _leftEyeImage;}

    void generate(int glassesType, int option, QColor backgroundColor);
private:
    QImage _rightEyeImage;
    QImage _leftEyeImage;
};

#endif // ANAGLYPH_H
