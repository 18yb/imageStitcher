#include "hog.h"

hog::hog(Mat img):
    src(img)
{

    cvtColor( src, src_gray, CV_BGR2GRAY );


    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( src.size(), CV_32FC1 );

    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    imShow("TEST", dst_norm);

}

