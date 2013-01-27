#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "anaglyph.h"
#include "qmatrixa.h"
#include "qvectora.h"

#include <QDebug>
#include <QPainter>

Anaglyph::Anaglyph()
    : _coordinateDistance(10), _observerDistance(80), _eyeDistance(40)
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
       // qDebug() << _data[i].print();
    }

}

void Anaglyph::generate(Ui::MainWindow *ui) {
    qDebug() << "Generating...";    

    int xmin= 9999;
    int xmax=-9999;
    int ymin= 9999;
    int ymax=-9999;

    int w = 800;
    int h = 400;

    int size = _data.size();

        for(int i = 0; i < size; ++i)
        {
            int x1 = _data.at(i).x1;
            int x2 = _data.at(i).x2;
            int y1 = _data.at(i).y1;
            int y2 = _data.at(i).y2;

            if( x1 > xmax )    xmax = x1;
            if( x2 > xmax )    xmax = x2;

            if( y1 > ymax )    ymax = y1;
            if( y2 > ymax )    ymax = y2;

            if( x1 < xmin )    xmin = x1;
            if( x2 < xmin )    xmin = x2;

            if( y1 < ymin )    ymin = y1;
            if( y2 < ymin )    ymin = y2;
        }

    //odleglosc od ramki
    int distFromEdge = _observerDistance;

    //ze swiata do ekranu
    QMatrixA wts = QMatrixA::worldToScreen(xmin, xmax, ymin, ymax, distFromEdge, w-distFromEdge, h-distFromEdge, distFromEdge);//h 0

    //przesuniecie
    QMatrixA tr  = QMatrixA::translation( 1,1,1 );

    //odleglosc od rzutni
    double d = 5.0;

    //perspektywa
    QMatrixA pv  = QMatrixA::perspectiveView(d);

    //skalowanie
    QMatrixA sc  = QMatrixA::scale(   ( 1.0 )/10 ,    ( 1.0 )/10 ,
                                    ( 1.0 )/10  );

    // rotacja
    QMatrixA rot = QMatrixA::rotate(   ui->sliderX->value(),
                                     ui->sliderY->value(),
                                    -ui->sliderZ->value() ); //- zeby obracalo sie do dodatniego kata

    //przesuniecie za rzutnie o pol maxymalnego zakresu
    QMatrixA tr_view  = QMatrixA::translation( 0, 0, ( -50 ) / 20.0 * 0.5);

    QMatrixA m1   = tr_view * tr * rot * sc;
    QMatrixA m2    = wts * pv;

    _anaglyphImage = QImage(QSize(w,h), QImage::Format_ARGB32_Premultiplied);
    _anaglyphImage.fill(_backgroundColor);

    QPainter p;
    p.begin(&_anaglyphImage);

    QVectorA w1;    //begin
    QVectorA w2;    //end

    for(int i = 0; i < size; ++i)
    {

        w1.Set(_data.at(i).x1, _data.at(i).y1, -_data.at(i).z1);
        w2.Set(_data.at(i).x2, _data.at(i).y2, -_data.at(i).z2);

        QMatrixA::line2d(m1, &w1, &w2);
        QMatrixA::line2d(m2, &w1, &w2);

        Point2D leftBegP = leftEyeView(w1.GetX(), w1.GetY(), w1.GetZ());
        Point2D leftEndP = leftEyeView(w2.GetX(), w2.GetY(), w2.GetZ());
        Point2D rightBegP = rightEyeView(w1.GetX(), w1.GetY(), w1.GetZ());
        Point2D rightEndP = rightEyeView(w2.GetX(), w2.GetY(), w2.GetZ());

        p.setPen(QPen(QColor(_colorLeftEye)));
        p.setBrush(QBrush(QColor(_colorLeftEye), Qt::NoBrush));
        p.drawLine(QLine(leftBegP.x, leftBegP.y, leftEndP.x, leftEndP.y));

        p.setPen(QPen(QColor(_colorRightEye)));
        p.setBrush(QBrush(QColor(_colorRightEye), Qt::NoBrush));
        p.drawLine(QLine(rightBegP.x, rightBegP.y, rightEndP.x, rightEndP.y));

    }

    p.end();


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
