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
}

void MainWindow::on_pushButton_2_clicked()
{
    _rightEyeImage = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpeg *.bmp *.jpg)"));
    qDebug() << _rightEyeImage;
    _a.addRightEyeImage(_rightEyeImage);
}
