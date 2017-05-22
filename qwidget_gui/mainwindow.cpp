#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(0,800,this));

    ui->lineEdit->setText(QString::number(400));
    ui->horizontalSlider->setValue(400);
    threshold = 400;

    surf_method = new surf(threshold);
    sift_method = new sift(threshold);
    hog_method = new hog(threshold);
    harris_method = new harris(threshold);

    counter =0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
    threshold=value;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->horizontalSlider->setValue(arg1.toInt());
    threshold = arg1.toInt();
}

void MainWindow::on_pushButton_clicked(){

    counter++;
    QStringList images = path.entryList(QStringList() << "*.jpg" << "*.JPG"<< "*.png"<< "*.PNG",QDir::Files);
    foreach(QString filename, images) {
        resources.push_back(imread((path.absolutePath()+"/"+filename).toStdString(),1));
        //qDebug()<<filename;
    }

    while(!resources.isEmpty()){

        refImage1 = resources.first();
        resources.pop_front();

        refImage2 = resources.first();
        resources.pop_front();


        if(ui->radioButton->isChecked()){
            roberts *r1= new roberts(refImage1);
            roberts *r2 = new roberts(refImage2);

            Mat image1 = r1->img;
            Mat image2 = r2->img;

            //vectorCompare = surf_method->startComparingRows(image1,image2, refImage1, refImage2);
            //vectorCompare = sift_method->startComparingRows(image1,image2, refImage1, refImage2);
            vectorCompare = harris_method->startComparingRows(image1,image2, refImage1, refImage2);
        }
        if(ui->radioButton_2->isChecked()){
            sobel *s1 = new sobel(refImage1);
            sobel *s2 = new sobel(refImage2);

            Mat image1 = s1->img;
            Mat image2 = s2->img;

            vectorCompare = surf_method->startComparingRows(image1,image2, refImage1, refImage2);
        }
        if(ui->radioButton_3->isChecked()){
            laplacian *l1 = new laplacian(refImage1);
            laplacian *l2 = new laplacian(refImage2);

            Mat image1 = l1->img;
            Mat image2 = l2->img;

            vectorCompare = surf_method->startComparingRows(image1,image2, refImage1, refImage2);
        }
        if(ui->radioButton_4->isChecked()){
            laplaciangaussian *log1 = new laplaciangaussian(refImage1);
            laplaciangaussian *log2 = new laplaciangaussian(refImage2);

            Mat image1 = log1->img;
            Mat image2 = log2->img;

            vectorCompare = surf_method->startComparingRows(image1,image2, refImage1, refImage2);
        }
        if(ui->radioButton_5->isChecked()){
            Mat gray_image1;
            Mat gray_image2;

            cvtColor( refImage1, gray_image1, CV_RGB2GRAY );
            cvtColor( refImage1, gray_image2, CV_RGB2GRAY );

            vectorCompare = surf_method->startComparingRows(gray_image1,gray_image2, refImage1, refImage2);
        }
    }
    QString writePath = path.absolutePath();
    imwrite(writePath.toStdString()+"/output"+to_string(counter)+".png", vectorCompare);

    QMessageBox::information(
        this,
        tr("File generated"),
        tr("File has been saved to: ")+path.absolutePath());

}


void MainWindow::on_pushButton_2_clicked()
{
    path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly);
}
