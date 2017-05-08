#include "laplacian.h"

laplacian::laplacian(Mat a_img):
    img(a_img)
{

    cvtColor(img, img, CV_BGR2GRAY);
    Mat result_img_1;
    Mat result_img_2;


    float m[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
    float m2[9] = {1, 1, 1, 1, -8, 1, 1, 1, 1};


    Mat kernel_1(Size(3,3), CV_32F, m);
    Mat kernel_2(Size(3,3), CV_32F, m2);

    filter2D(img, result_img_1, -1, kernel_1);
    filter2D(img, result_img_2, -1 , kernel_2);

    //imshow( "Image", img);

    /*namedWindow("Laplacian1", WINDOW_NORMAL );
    resizeWindow("Laplacian1",500,350);

    namedWindow("Laplacian2", WINDOW_NORMAL );
    resizeWindow("Laplacian2",500,350);

    imshow("Laplacian1", result_img_1);
    imshow("Laplacian2", result_img_2);*/

}
