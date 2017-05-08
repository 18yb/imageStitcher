#include "laplaciangaussian.h"

laplaciangaussian::laplaciangaussian(Mat a_img):
    img(a_img)
{

    cvtColor(img, img, CV_BGR2GRAY);

    Mat laplacian;
    Mat blurred_img;
    GaussianBlur(img, blurred_img, Size (5,5), 1.5 );
    Laplacian(blurred_img, laplacian, -1, 3 );


    //imshow( "Image", img);
    //imshow( "Result blur", blurred_img);
    /*namedWindow("Laplacian of Gaussian", WINDOW_NORMAL );
    resizeWindow("Laplacian of Gaussian",500,350);
    imshow("Laplacian of Gaussian", laplacian);*/

}
