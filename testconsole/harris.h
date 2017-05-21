#ifndef HARRIS_H
#define HARRIS_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv::xfeatures2d;
using namespace cv;


class harris
{
public:
    harris(Mat img);

private:
    int thresh = 200;
    int max_thresh = 255;
    Mat src, src_gray;
};

#endif // HARRIS_H
