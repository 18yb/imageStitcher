#ifndef STITCHER_H
#define STITCHER_H

#include <stdio.h>
#include <iostream>
#include <QDebug>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QDir>


#include <roberts.h>
#include <sobel.h>
#include <laplacian.h>
#include <laplaciangaussian.h>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


class Stitcher
{
public:
    Stitcher(int arg);
    Mat translateImage(Mat &img, Mat dst_size, int offset_x, int offset_y);
    vector<KeyPoint> detectKeypoints(Mat image);
    Mat computeKeypoints(Mat image, vector<KeyPoint> keypoints);
    Mat reverseComparison(Mat image1, Mat image2, Mat refImage1, Mat refImage2);
    Mat startComparingRows(Mat image1, Mat image2, Mat refImage1, Mat refImage2);


    Mat getFinalMat();

private:
    Mat output;
    int threshold_value;
};

#endif // STITCHER_H
