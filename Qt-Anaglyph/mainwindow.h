#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QDebug>
#include "anaglyph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_buttonColor_clicked();

    void on_buttonLoad_clicked();

    void on_buttonSave_clicked();

    void on_comboBoxGlasses_currentIndexChanged(int index);

    void on_buttonColorLeftEye_clicked();

    void on_buttonColorRightEye_clicked();

    void on_sliderX_valueChanged(int value);

    void on_sliderY_valueChanged(int value);

    void on_sliderZ_valueChanged(int value);

    void on_horizontalSlider_valueChanged(int value);

private:
    void updateAnaglyph();

    Ui::MainWindow *ui;
    Anaglyph _a;

    QColor _backgroundColor;
    QColor _colorLeftEye;
    QColor _colorRightEye;
    bool isDataLoaded;
};

#endif // MAINWINDOW_H
