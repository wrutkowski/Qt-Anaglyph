#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "anaglyph.h"
#include "qmatrixa.h"
#include "qvectora.h"

#include <QDebug>
#include <QPainter>

Anaglyph::Anaglyph()
    : _coordinateDistance(10), _observerDistance(80), _eyeDistance(40), _zoom(10), _lineWidth(1.0), _drawAnaglyph(true), _drawShape(true)
{
    _X = 0;
    _Y = 0;
    _Z = 0;
    _backgroundColor = Qt::white;
}

void Anaglyph::setData(QStringList data) {
    _data.clear();

    // axis X, Y, Z - testing
    ALine alineX(-100, 0, 0, 100, 0, 0);
    ALine alineY(0, -100, 0, 0, 100, 0);
    ALine alineZ(0, 0, -100, 0, 0, -100);
    _data.append(alineX);
    _data.append(alineY);
    _data.append(alineZ);
    // ---

    for (int i = 0; i < data.size(); ++i) {
        QString line = data[i];
        if (line.length() == 0 || QString(line[0]) == "#") continue; // empty or comment

        QStringList row = line.split(" ");

        // first row should have only rows count
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

    int xmin =  9999;
    int xmax = -9999;
    int ymin =  9999;
    int ymax = -9999;

    int w = 800;
    int h = 600;

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

            if(size == 1){
                xmin = x1;
                xmax = x2;
                ymin = y1;
                ymax = y2;

            }

        }

    // distance form edge of image
    int distFromEdge = 10;

    // world to screen
    QMatrixA wts = QMatrixA::worldToScreen(xmin, xmax, ymin, ymax, distFromEdge, w-distFromEdge, h-distFromEdge, distFromEdge);

    // translation
    QMatrixA tr  = QMatrixA::translation( 1,1,1 );

    // perspective
    QMatrixA pv  = QMatrixA::perspectiveView(20.0);

    // scale = zoom
    QMatrixA sc  = QMatrixA::scale( _zoom/100.0, _zoom/100.0, _zoom/100.0);

    // rotation
    QMatrixA rot = QMatrixA::rotate(   ui->sliderX->value(), ui->sliderY->value(), -ui->sliderZ->value() ); // minus (-) to inverse Z rotate for plus angle

    // translation
    QMatrixA tr_view  = QMatrixA::translation( -1, -1, -1);

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

        if (_drawShape) {
            p.setPen(QPen(QColor(Qt::black)));
            p.drawLine(QLine(w1.GetX(), w1.GetY(), w2.GetX(), w2.GetY()));
        }

        if (_drawAnaglyph) {
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
        }
    }

    p.end();

}
Point2D Anaglyph::leftEyeView(double x, double y, double z)
{
    Point2D p;
    p.y = y*_observerDistance/(_observerDistance+_coordinateDistance+z);
    p.x = ((x*_observerDistance)/(_coordinateDistance+_observerDistance+z))-
            ((_eyeDistance*(_coordinateDistance+z))/(_coordinateDistance+_observerDistance+z));
    qDebug() << "Z" << z;
    return p;
}

Point2D Anaglyph::rightEyeView(double x, double y, double z)
{
    Point2D p;
    p.y = y*_observerDistance/(_observerDistance+_coordinateDistance+z);
    p.x = ((x*_observerDistance)/(_coordinateDistance+_observerDistance+z))+
            ((_eyeDistance*(_coordinateDistance+z))/(_coordinateDistance+_observerDistance+z));
    qDebug() << "Z" << z;
    return p;
}
