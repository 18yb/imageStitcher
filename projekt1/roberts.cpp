#include "roberts.h"

roberts::roberts(Mat a_img):
    img(a_img)
{


  cvtColor(img, img, CV_BGR2GRAY);
  Mat result_img_1;
  Mat result_img_2;
  Mat result_img_3;


  float m[4] = {1, 0, 0, -1};
  float m2[4] = {0, -1, 1, 0};


  Mat kernel_1(Size(2,2), CV_32F, m);
  Mat kernel_2(Size(2,2), CV_32F, m2);

  filter2D(img, result_img_1, -1, kernel_1);
  filter2D(img, result_img_2, -1 , kernel_2);

  result_img_3 = abs(result_img_1)+abs(result_img_2);

  img = result_img_3;
  /*namedWindow("Roberts", WINDOW_NORMAL );
  resizeWindow("Roberts",500,350);
  imshow("Roberts", result_img_3);*/


}
