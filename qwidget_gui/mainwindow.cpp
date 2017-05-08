#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(0,255,this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->horizontalSlider->setValue(arg1.toInt());
}

void MainWindow::on_pushButton_clicked()
{

}
