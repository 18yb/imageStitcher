#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <surf.h>
#include <QFileDialog>
#include <QTreeView>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setValue(int);

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    surf *surf_method;
    Mat vectorCompare;
    Mat refImage1;
    Mat refImage2;
    QVector<Mat> resources;
    int threshold;
    QDir path;
    int counter;


};

#endif // MAINWINDOW_H
