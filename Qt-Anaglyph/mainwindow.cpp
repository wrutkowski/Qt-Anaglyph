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
    _leftEyeImage = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpeg *.bmp *.jpg)"));
    _a.addLeftEyeImage(_leftEyeImage);
    QImage imgCpy = _a.getLeftEyeImage();
    float wProp = 1.0f;
    if(imgCpy.width() > ui->label_3->width())
        wProp = imgCpy.width() / ui->label_3->width();
    else if(imgCpy.width() < ui->label_3->width())
        wProp = ui->label_3->width() / imgCpy.width();
    ui->label_3->setPixmap(QPixmap::fromImage(imgCpy.scaled(QSize(ui->label_3->width()/wProp, ui->label_3->height()))));
}

void MainWindow::on_pushButton_2_clicked()
{
    _rightEyeImage = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpeg *.bmp *.jpg)"));
    _a.addRightEyeImage(_rightEyeImage);
    QImage imgCpy = _a.getRigthEyeImage();
    float wProp = 1.0f;
    if(imgCpy.width() > ui->label_2->width())
        wProp = imgCpy.width() / ui->label_2->width();
    else if(imgCpy.width() < ui->label_2->width())
        wProp = ui->label_2->width() / imgCpy.width();
    ui->label_2->setPixmap(QPixmap::fromImage(imgCpy.scaled(QSize(ui->label_2->width()/wProp, ui->label_2->height()))));
}
