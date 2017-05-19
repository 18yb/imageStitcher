#ifndef HOG_H
#define HOG_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

class hog
{
public:
    hog(Mat img);

private:
    int thresh = 200;
    int max_thresh = 255;
    Mat src, src_gray;

};

#endif // HOG_H
