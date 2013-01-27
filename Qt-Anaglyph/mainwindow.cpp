#include "mainwindow.h"
#include "ui_mainwindow.h"

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
        _a.setLeftEyeColor(_colorLeftEye);
        _a.setRightEyeColor(_colorRightEye);
        _a.generate(ui);
        _a.setLineWidth(_lineWidth);

        ui->imageAnaglyph->setPixmap(QPixmap::fromImage(_a.getGeneratedAnaglyph()));
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

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->labelLine->setText(QString::number(value));
    _lineWidth = (double)value/10.0;
    this->updateAnaglyph();
}
