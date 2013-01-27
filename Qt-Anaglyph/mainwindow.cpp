#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qmatrixa.h"
#include "qvectora.h"
#include <QPainter>
#include <QMessageBox>
#include <QImageWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _backgroundColor = Qt::white;
    _colorLeftEye = QColor(Qt::red);
    _colorRightEye = QColor(Qt::cyan);
    ui->labelColor->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_backgroundColor.red()).arg(_backgroundColor.green()).arg(_backgroundColor.blue()));
    ui->labelColorEyeLeft->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorLeftEye.red()).arg(_colorLeftEye.green()).arg(_colorLeftEye.blue()));
    ui->labelColorEyeRight->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorRightEye.red()).arg(_colorRightEye.green()).arg(_colorRightEye.blue()));

    isDataLoaded = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonColor_clicked()
{
    _backgroundColor = QColorDialog::getColor(Qt::white, this, tr("Select Color"), 0);

    ui->labelColor->setHidden(false);
    ui->labelColor->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_backgroundColor.red()).arg(_backgroundColor.green()).arg(_backgroundColor.blue()));
    this->updateAnaglyph();
}

void MainWindow::on_buttonLoad_clicked()
{

    QString dataFilename = QFileDialog::getOpenFileName(this, tr("Open Shape Data"), "", tr("Data Files (*.dat *.txt)"));

    QFile file(dataFilename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", file.errorString());
    }
    ui->textEdit->clear();

    QTextStream in(&file);

    QStringList adata;

    while(!in.atEnd()) {
        QString line = in.readLine();
        adata.append(line);
        ui->textEdit->append(line);
    }

    file.close();

    _a.setData(adata);
    isDataLoaded = true;

    this->updateAnaglyph();

}

void MainWindow::on_buttonSave_clicked()
{
    QImage anaglyphImg = _a.getGeneratedAnaglyph();
    if (anaglyphImg.isNull()) {
        QMessageBox::information(0, "Error", "Load valid data file first!");
        return;
    }

    QString imageFilename = QFileDialog::getSaveFileName(this, tr("Save Anaglyph Image"), "", tr("Image File (*.png)"));
    QImageWriter writer(imageFilename);
    if (!writer.write(anaglyphImg)) {
        QMessageBox::information(0, "Error", writer.errorString());
    }
}

void MainWindow::updateAnaglyph() {
    if (isDataLoaded) {
        _a.setBackgroundColor(_backgroundColor);
        _a.setGlassesColors(_colorLeftEye, _colorRightEye);
        _a.setAxis(ui->sliderX->value(), ui->sliderY->value(), ui->sliderZ->value());
        _a.generate();


        QImage img = _a.getGeneratedAnaglyph();

        int xmin= 9999;
        int xmax=-9999;
        int ymin= 9999;
        int ymax=-9999;

        int w = 400;
        int h = 400;

        int size = _a.getData().size();
        QList<ALine> cont = _a.getData();

            for(int i = 0; i < size; ++i)
            {
                int x1 = cont.at(i).x1;
                int x2 = cont.at(i).x2;
                int y1 = cont.at(i).y1;
                int y2 = cont.at(i).y2;

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
        int distFromEdge = _a.getObservDist();  //to mozna wrzucic na formatke
        //ze swiata do ekranu
            QMatrixA wts = QMatrixA::worldToScreen(xmin, xmax, ymin, ymax, distFromEdge, w-distFromEdge, h-distFromEdge, distFromEdge);//h 0
        //przesuniecie

            QMatrixA tr  = QMatrixA::translation( 1,1,1 );
        //odleglosc od rzutni
            double d = 5.0;
        //perspektywa
            QMatrixA pv  = QMatrixA::perspectiveView(d);
        //skalowanie

            QMatrixA sc  = QMatrixA::scale(   ( 1.0 )/10 ,
                                            ( 1.0 )/10 ,
                                            ( 1.0 )/10  );


        /*
        //obrot
            Matrix4 rOX = Matrix4::rotateOX( WxSB_RotateX->GetThumbPosition() );
            Matrix4 rOY = Matrix4::rotateOY( WxSB_RotateY->GetThumbPosition() );
            Matrix4 rOZ = Matrix4::rotateOZ( WxSB_RotateZ->GetThumbPosition() );

            Matrix4 rE      = Matrix4::rotateEuler( 90, WxSB_RotateY->GetThumbPosition(), -90 );
            Matrix4 rE_inv  = Matrix4::rotateEuler(-90, WxSB_RotateY->GetThumbPosition(), 90 );
        */
            QMatrixA rot = QMatrixA::rotate(   ui->sliderX->value(),
                                             ui->sliderY->value(),
                                            -ui->sliderZ->value() ); //- zeby obracalo sie do dodatniego kata
        //przesuniecie za rzutnie o pol maxymalnego zakresu
            QMatrixA tr_view  = QMatrixA::translation( 0, 0, ( -50 ) / 20.0 * 0.5);

        //ostateczna macierz przeksztalcenia
        //    Matrix4 T = wts * pv * tr * rot * sc;
        //    Matrix4 T = wts * pv * tr * rE_inv * rOX * rE * rOY * sc;
        //    Matrix4 T = wts * pv * tr * rE_inv * rE * sc;

            QMatrixA m1   = tr_view * tr * rot * sc;
            QMatrixA m2    = wts * pv;

            //QMatrixA m1   = rot;
            //QMatrixA m2    = wts * pv;


            img = QImage(QSize(400,400), QImage::Format_ARGB32_Premultiplied);
            img.fill(_a.getBackgroundColor());   // fill  _rowImg with Qt:color0
            //qDebug() << _a.getBackgroundColor();
            QPainter p;
            p.begin(&img);

            QVectorA w1;    //begin
            QVectorA w2;    //end

            for(int i = 0; i < size; ++i)
            {
                /*ALine t = cont.at(i);
                qDebug() << "T:";
                qDebug() << cont.at(i).x1 << " " << cont.at(i).y1 << " " << cont.at(i).z1;
                qDebug() << "....";*/

                w1.Set(cont.at(i).x1, cont.at(i).y1, -cont.at(i).z1);
                w2.Set(cont.at(i).x2, cont.at(i).y2, -cont.at(i).z2);
               // w1.Print();
                QMatrixA::line2d(m1, &w1, &w2);
                QMatrixA::line2d(m2, &w1, &w2);
                Point2D leftBegP = _a.leftEyeView(w1.GetX(), w1.GetY(), w1.GetZ());
                Point2D leftEndP = _a.leftEyeView(w2.GetX(), w2.GetY(), w2.GetZ());
                Point2D rightBegP = _a.rightEyeView(w1.GetX(), w1.GetY(), w1.GetZ());
                Point2D rightEndP = _a.rightEyeView(w2.GetX(), w2.GetY(), w2.GetZ());
                p.setPen(QPen(QColor(Qt::blue)));
                p.setBrush(QBrush(QColor(Qt::blue), Qt::NoBrush));
                p.drawLine(QLine(leftBegP.x, leftBegP.y, leftEndP.x, leftEndP.y));
                p.setPen(QPen(QColor(Qt::red)));
                p.setBrush(QBrush(QColor(Qt::red), Qt::NoBrush));
                p.drawLine(QLine(rightBegP.x, rightBegP.y, rightEndP.x, rightEndP.y));


            }

            p.end();

            ui->imageAnaglyph->setPixmap(QPixmap::fromImage(img));
            ui->imageAnaglyph->show();


    }
}

void MainWindow::on_comboBoxGlasses_currentIndexChanged(int index)
{
    if (index == 3) { // custom
        ui->buttonColorLeftEye->setEnabled(true);
        ui->buttonColorRightEye->setEnabled(true);
    } else {
        ui->buttonColorLeftEye->setEnabled(false);
        ui->buttonColorRightEye->setEnabled(false);
    }

    switch (index) {
        case 0:
            _colorLeftEye = QColor(Qt::red);
            _colorRightEye = QColor(Qt::cyan);
            break;
        case 1:
            _colorLeftEye = QColor(Qt::red);
            _colorRightEye = QColor(Qt::blue);
            break;
        case 2:
            _colorLeftEye = QColor(Qt::red);
            _colorRightEye = QColor(Qt::green);
            break;
    }

    ui->labelColorEyeLeft->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorLeftEye.red()).arg(_colorLeftEye.green()).arg(_colorLeftEye.blue()));
    ui->labelColorEyeRight->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorRightEye.red()).arg(_colorRightEye.green()).arg(_colorRightEye.blue()));
    this->updateAnaglyph();
}

void MainWindow::on_buttonColorLeftEye_clicked()
{
    _colorLeftEye = QColorDialog::getColor(_colorLeftEye, this, tr("Select Left Eye Glass Color"), 0);
    ui->labelColorEyeLeft->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorLeftEye.red()).arg(_colorLeftEye.green()).arg(_colorLeftEye.blue()));
    ui->labelColorEyeRight->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorRightEye.red()).arg(_colorRightEye.green()).arg(_colorRightEye.blue()));
    this->updateAnaglyph();
}

void MainWindow::on_buttonColorRightEye_clicked()
{
    _colorRightEye = QColorDialog::getColor(_colorRightEye, this, tr("Select Right Eye Glass Color"), 0);
    ui->labelColorEyeLeft->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorLeftEye.red()).arg(_colorLeftEye.green()).arg(_colorLeftEye.blue()));
    ui->labelColorEyeRight->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorRightEye.red()).arg(_colorRightEye.green()).arg(_colorRightEye.blue()));
    this->updateAnaglyph();
}

void MainWindow::on_sliderX_valueChanged(int value)
{
    ui->labelX->setText(QString("%1°").arg(value));
    this->updateAnaglyph();
}

void MainWindow::on_sliderY_valueChanged(int value)
{
    ui->labelY->setText(QString("%1°").arg(value));
    this->updateAnaglyph();
}

void MainWindow::on_sliderZ_valueChanged(int value)
{
    ui->labelZ->setText(QString("%1°").arg(value));
    this->updateAnaglyph();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->labelZoom->setText(QString::number(value));
    _a.setCoordDist(value);
    this->updateAnaglyph();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->labelEye->setText(QString::number(value));
    _a.setEyeDist(value);
    this->updateAnaglyph();
}
