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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_buttonColor_clicked();

    void on_buttonGenerate_clicked();

    void on_radioButtonBackground_clicked();

    void on_radioButtonCrop_clicked();

private:
    Ui::MainWindow *ui;
    QString _leftEyeImage;
    QString _rightEyeImage;
    Anaglyph _a;
    QColor _backgroundColor;
};

#endif // MAINWINDOW_H
