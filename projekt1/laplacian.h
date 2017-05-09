#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class laplacian
{
public:
    laplacian(Mat a_img);
    Mat img;
};

#endif // LAPLACIAN_H
