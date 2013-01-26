#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    _leftEyeImage = QFileDialog::getOpenFileName(this, tr("Open Left Eye Image"), "", tr("Image Files (*.png *.jpeg *.bmp *.jpg)"));
    _a.addLeftEyeImage(_leftEyeImage);
    QImage imgCpy = _a.getLeftEyeImage();
    float wProp = 1.0f;
    if(imgCpy.width() > ui->imageEyeLeft->width())
        wProp = imgCpy.width() / ui->imageEyeLeft->width();
    else if(imgCpy.width() < ui->imageEyeLeft->width())
        wProp = ui->imageEyeLeft->width() / imgCpy.width();
    ui->imageEyeLeft->setPixmap(QPixmap::fromImage(imgCpy.scaled(QSize(ui->imageEyeLeft->width()/wProp, ui->imageEyeLeft->height()))));
}

void MainWindow::on_pushButton_2_clicked()
{
    _rightEyeImage = QFileDialog::getOpenFileName(this, tr("Open Right Eye Image"), "", tr("Image Files (*.png *.jpeg *.bmp *.jpg)"));
    _a.addRightEyeImage(_rightEyeImage);
    QImage imgCpy = _a.getRigthEyeImage();
    float wProp = 1.0f;
    if(imgCpy.width() > ui->imageEyeRight->width())
        wProp = imgCpy.width() / ui->imageEyeRight->width();
    else if(imgCpy.width() < ui->imageEyeRight->width())
        wProp = ui->imageEyeRight->width() / imgCpy.width();
    ui->imageEyeRight->setPixmap(QPixmap::fromImage(imgCpy.scaled(QSize(ui->imageEyeRight->width()/wProp, ui->imageEyeRight->height()))));
}

void MainWindow::on_buttonColor_clicked()
{
    _backgroundColor = QColorDialog::getColor(Qt::white, this, tr("Select Color"), 0);

    ui->labelColor->setHidden(false);
    ui->labelColor->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_backgroundColor.red()).arg(_backgroundColor.green()).arg(_backgroundColor.blue()));
}

void MainWindow::on_buttonGenerate_clicked()
{
    _a.generate(ui->comboBoxGlasses->currentIndex(), (ui->radioButtonCrop->isChecked() ? kAnaglyphOptionCrop : kAnaglyphOptionBackground), _backgroundColor);
}

void MainWindow::on_radioButtonBackground_clicked()
{
    ui->buttonColor->setEnabled(true);
    ui->labelColor->setHidden(false);
}

void MainWindow::on_radioButtonCrop_clicked()
{
    ui->buttonColor->setEnabled(false);
    ui->labelColor->setHidden(true);
}
