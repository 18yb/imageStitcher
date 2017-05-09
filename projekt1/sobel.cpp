#include "sobel.h"

sobel::sobel(Mat a_img):
    img(a_img)
{


    cvtColor(img, img, CV_BGR2GRAY);
     Mat result_img_1;
     Mat result_img_2;
     Mat result_img_3;

     Sobel( img, result_img_1, -1 ,1 ,0 );
     Sobel( img, result_img_2, -1 ,0 ,1 );
     result_img_3 = abs(result_img_1)+abs(result_img_2);

     img = result_img_3;

     //imshow( "Image", img);
     //imshow( "Result 1", result_img_1);
    // imshow( "Result 2", result_img_2);

    /* namedWindow("Sobel", WINDOW_NORMAL );
     resizeWindow("Sobel",500,350);
     imshow("Sobel", result_img_3);*/
}
