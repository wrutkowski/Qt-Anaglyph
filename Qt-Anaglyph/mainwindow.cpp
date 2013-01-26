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
    qDebug() << _leftEyeImage;
    _a.addLeftEyeImage(_leftEyeImage);
    QImage imgCpy = _a.getLeftEyeImage();
    ui->label_3->setPixmap(QPixmap::fromImage(imgCpy.scaled(QSize(ui->label_3->width(), ui->label_3->height()))));
}

void MainWindow::on_pushButton_2_clicked()
{
    _rightEyeImage = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpeg *.bmp *.jpg)"));
    qDebug() << _rightEyeImage;
    _a.addRightEyeImage(_rightEyeImage);
    QImage imgCpy = _a.getRigthEyeImage();
    ui->label_2->setPixmap(QPixmap::fromImage(imgCpy.scaled(QSize(ui->label_2->width(), ui->label_2->height()))));
}
