#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "anaglyph.h"
#include "qmatrixa.h"
#include "qvectora.h"

#include <QDebug>
#include <QPainter>

Anaglyph::Anaglyph()
    : _coordinateDistance(-10), _observerDistance(80), _eyeDistance(40)
{
    _X = 0;
    _Y = 0;
    _Z = 0;
    _backgroundColor = Qt::white;
    _lineWidth = 1.0;
}

void Anaglyph::setData(QStringList data) {
    _data.clear();

    for (int i = 0; i < data.size(); ++i) {
        QString line = data[i];
        if (line.length() == 0 || QString(line[0]) == "#") continue; // empty or comment

        QStringList row = line.split(" ");

        //gdy pierwsza linia ma tylko ilosc krawedzi
        if (row.count() < 7 && i != 0) continue;

        int type = row[0].toInt();

        if (type == 1 && i != 0) {
            ALine aline(row[1].toInt(), row[2].toInt(), row[3].toInt(), row[4].toInt(), row[5].toInt(), row[6].toInt());
            _data.append(aline);
        }

    }

    for (int i = 0; i < _data.size(); ++i) {
       qDebug() << _data[i].print();
    }

}

void Anaglyph::generate(Ui::MainWindow *ui) {
    qDebug() << "Generating...";

    int xmin= 9999;
    int xmax=-9999;
    int ymin= 9999;
    int ymax=-9999;
    int zmin= 9999;
    int zmax=-9999;

    int w = 800;
    int h = 600;

    int size = _data.size();

        for(int i = 0; i < size; ++i)
        {
            int x1 = _data.at(i).x1;
            int x2 = _data.at(i).x2;
            int y1 = _data.at(i).y1;
            int y2 = _data.at(i).y2;
            int z1 = _data.at(i).z1;
            int z2 = _data.at(i).z2;

            if( x1 > xmax )    xmax = x1;
            if( x2 > xmax )    xmax = x2;

            if( y1 > ymax )    ymax = y1;
            if( y2 > ymax )    ymax = y2;

            if( z1 > zmax )    zmax = z1;
            if( z2 > zmax )    zmax = z2;

            if( x1 < xmin )    xmin = x1;
            if( x2 < xmin )    xmin = x2;

            if( y1 < ymin )    ymin = y1;
            if( y2 < ymin )    ymin = y2;

            if( z1 < zmin )    zmin = z1;
            if( z2 < zmin )    zmin = z2;

            if(size == 1){
                xmin = x1;
                xmax = x2;
                ymin = y1;
                ymax = y2;
                zmin = z1;
                zmax = z2;

            }

        }

       // qDebug() << xmin << " " << xmax <<":"<<ymin<<" "<<ymax;
    //odleglosc od ramki
    int distFromEdge = _observerDistance;
    int coor = -_coordinateDistance;
   // qDebug() << _coordinateDistance << " " << _observerDistance;
    //ze swiata do ekranu
    QMatrixA wts = QMatrixA::worldToScreen(xmin, xmax, ymin, ymax, zmin, zmax, distFromEdge, w-distFromEdge, h-distFromEdge, distFromEdge, 0, distFromEdge);//h 0

    //przesuniecie
    QMatrixA tr  = QMatrixA::translation( 1,1,1 );

    //odleglosc od rzutni
    double d = 20.0;

    //perspektywa
    QMatrixA pv  = QMatrixA::perspectiveView(d);

    //skalowanie
    QMatrixA sc  = QMatrixA::scale( (coor+1.0)/50, (coor+1.0)/50, (coor+1.0)/50 );

    // rotacja
    QMatrixA rot = QMatrixA::rotate(   ui->sliderX->value(),
                                     ui->sliderY->value(),
                                    -ui->sliderZ->value() ); //- zeby obracalo sie do dodatniego kata

    //przesuniecie za rzutnie
    QMatrixA tr_view  = QMatrixA::translation( 0, 0, 0.5);

    QMatrixA m1   = tr_view * tr * rot * sc;
    QMatrixA m2    = wts * pv;
    wts.Print();


    _anaglyphImage = QImage(QSize(w,h), QImage::Format_ARGB32_Premultiplied);
    _anaglyphImage.fill(_backgroundColor);

    QPainter p;
    p.begin(&_anaglyphImage);

    QVectorA w1;    //begin
    QVectorA w2;    //end

    QPen pen1 = QPen(QColor(_colorLeftEye));
    QPen pen2 = QPen(QColor(_colorRightEye));
    pen1.setWidthF(_lineWidth);
    pen2.setWidthF(_lineWidth);

    for(int i = 0; i < size; ++i)
    {

        w1.Set(_data.at(i).x1, _data.at(i).y1, -_data.at(i).z1);
        w2.Set(_data.at(i).x2, _data.at(i).y2, -_data.at(i).z2);

        QMatrixA::line2d(m1, &w1, &w2);
        QMatrixA::line2d(m2, &w1, &w2);

        //if(_drawAnaglyph){
            Point2D leftBegP = leftEyeView(w1.GetX(), w1.GetY(), w1.GetZ());
            Point2D leftEndP = leftEyeView(w2.GetX(), w2.GetY(), w2.GetZ());
            Point2D rightBegP = rightEyeView(w1.GetX(), w1.GetY(), w1.GetZ());
            Point2D rightEndP = rightEyeView(w2.GetX(), w2.GetY(), w2.GetZ());

            p.setPen(pen1);
            p.setBrush(QBrush(QColor(_colorLeftEye), Qt::NoBrush));
            p.drawLine(QLine(leftBegP.x, leftBegP.y, leftEndP.x, leftEndP.y));

            p.setPen(pen2);
            p.setBrush(QBrush(QColor(_colorRightEye), Qt::NoBrush));
            p.drawLine(QLine(rightBegP.x, rightBegP.y, rightEndP.x, rightEndP.y));

        /*}
        if(_drawShape){

            p.setPen(QPen(QColor(Qt::black)));
            p.setBrush(QBrush(QColor(_colorRightEye), Qt::NoBrush));
            p.drawLine(QLine(w1.GetX(), w1.GetY(), w2.GetX(), w2.GetY()));

        }*/

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
