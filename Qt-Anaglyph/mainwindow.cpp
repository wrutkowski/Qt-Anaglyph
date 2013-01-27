#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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
}

void MainWindow::on_buttonLoad_clicked()
{

    QString dataFilename = QFileDialog::getOpenFileName(this, tr("Open Shape Data"), "", tr("Data Files (*.dat *.txt)"));

    QFile file(dataFilename);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString());
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

}

void MainWindow::updateAnaglyph() {
    if (isDataLoaded) {
        _a.setBackgroundColor(_backgroundColor);
        _a.setGlassesType(ui->comboBoxGlasses->currentIndex());
        _a.setAxis(ui->sliderX->value(), ui->sliderY->value(), ui->sliderZ->value());
        _a.generate();
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
}

void MainWindow::on_buttonColorLeftEye_clicked()
{
    _colorLeftEye = QColorDialog::getColor(_colorLeftEye, this, tr("Select Left Eye Glass Color"), 0);
    ui->labelColorEyeLeft->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorLeftEye.red()).arg(_colorLeftEye.green()).arg(_colorLeftEye.blue()));
    ui->labelColorEyeRight->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorRightEye.red()).arg(_colorRightEye.green()).arg(_colorRightEye.blue()));
}

void MainWindow::on_buttonColorRightEye_clicked()
{
    _colorRightEye = QColorDialog::getColor(_colorRightEye, this, tr("Select Right Eye Glass Color"), 0);
    ui->labelColorEyeLeft->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorLeftEye.red()).arg(_colorLeftEye.green()).arg(_colorLeftEye.blue()));
    ui->labelColorEyeRight->setStyleSheet(QString("QLabel { background-color: rgb(%1, %2, %3) }").arg(_colorRightEye.red()).arg(_colorRightEye.green()).arg(_colorRightEye.blue()));
}
