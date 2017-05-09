#ifndef LOG_H
#define LOG_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


class laplaciangaussian
{
public:
    laplaciangaussian(Mat a_img);
    Mat img;
};

#endif // LOG_H
