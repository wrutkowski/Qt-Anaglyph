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

private:
    void updateAnaglyph();

    Ui::MainWindow *ui;
    Anaglyph _a;

    QColor _backgroundColor;
    bool isDataLoaded;
};

#endif // MAINWINDOW_H
