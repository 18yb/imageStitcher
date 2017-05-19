#include <QCoreApplication>
#include <harris.h>
#include <sift.h>
#include <hog.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    hog *haris = new hog();

    return a.exec();
}
