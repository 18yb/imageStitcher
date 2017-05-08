#ifndef ROBERTS_H
#define ROBERTS_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class roberts
{
public:
    roberts(Mat a_img);
    Mat img;

};

#endif // ROBERTS_H
