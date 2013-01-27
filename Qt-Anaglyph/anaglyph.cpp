#include "anaglyph.h"
#include <QDebug>

Anaglyph::Anaglyph()
    : _coordinateDistance(5), _observerDistance(80), _eyeDistance(40)
{
    _X = 0;
    _Y = 0;
    _Z = 0;
    _backgroundColor = Qt::white;
}

void Anaglyph::setData(QStringList data) {
    _data.clear();

    for (int i = 0; i < data.size(); ++i) {
        QString line = data[i];
        if (line.length() == 0 || QString(line[0]) == "#") continue; // empty or comment

        QStringList row = line.split(" ");
        if (row.count() < 7) continue;

        int type = row[0].toInt();
        if (type == 1) {
            ALine aline(row[1].toInt(), row[2].toInt(), row[3].toInt(), row[4].toInt(), row[5].toInt(), row[6].toInt());
            _data.append(aline);
        }

    }

    for (int i = 0; i < _data.size(); ++i) {
        qDebug() << _data[i].print();
    }

}

void Anaglyph::generate() {
    qDebug() << "Generating...";


}
Point2D Anaglyph::leftEyeView(double x, double y, double z)
{
    Point2D p;
    p.y = y*_observerDistance/(_observerDistance+_coordinateDistance+z);
    p.x = ((x*_observerDistance)/(_coordinateDistance+_observerDistance+z))-
            ((_eyeDistance*(_coordinateDistance+z))/(_coordinateDistance+_observerDistance+z));
    return p;
}

Point2D Anaglyph::rightEyeView(double x, double y, double z)
{
    Point2D p;
    p.y = y*_observerDistance/(_observerDistance+_coordinateDistance+z);
    p.x = ((x*_observerDistance)/(_coordinateDistance+_observerDistance+z))+
            ((_eyeDistance*(_coordinateDistance+z))/(_coordinateDistance+_observerDistance+z));
    return p;
}
