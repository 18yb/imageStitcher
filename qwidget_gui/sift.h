#ifndef SIFT_H
#define SIFT_H


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

class sift
{
public:
    sift(int arg);
    Mat translateImage(Mat &img, Mat dst_size, int offset_x, int offset_y);
    vector<KeyPoint> detectKeypoints(Mat image);
    Mat computeKeypoints(Mat image, vector<KeyPoint> keypoints);
    Mat reverseComparison(Mat image1, Mat image2, Mat refImage1, Mat refImage2);
    Mat startComparingRows(Mat image1, Mat image2, Mat refImage1, Mat refImage2);
    void overlayImage(Mat* src, Mat* overlay, const Point& location);

    Mat alpha(Mat img);

    Mat getFinalMat();

private:
    Mat output;
    int threshold_value;

};

#endif // SIFT_H
