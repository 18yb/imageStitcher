#ifndef SOBEL_H
#define SOBEL_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class sobel
{
public:
    sobel(Mat a_img);
    Mat img;
};

#endif // SOBEL_H
