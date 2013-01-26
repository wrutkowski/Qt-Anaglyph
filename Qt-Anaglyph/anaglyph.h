#ifndef ANAGLYPH_H
#define ANAGLYPH_H

#include <QImage>

class Anaglyph
{
public:
    Anaglyph();
    bool addLeftEyeImage(const QString& fileName, const char* format=0) {return _leftEyeImage.load(fileName, format);}
    bool addRightEyeImage(const QString& fileName, const char* format=0) {return _rightEyeImage.load(fileName, format);}
    QImage getRigthEyeImage() const {return _rightEyeImage;}
    QImage getLeftEyeImage() const {return _leftEyeImage;}
private:
    QImage _rightEyeImage;
    QImage _leftEyeImage;
};

#endif // ANAGLYPH_H
