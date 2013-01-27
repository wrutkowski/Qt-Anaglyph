#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _backgroundColor = Qt::white;
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
