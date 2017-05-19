#include "harris.h"
#include <QDebug>
#define HARRIS SURF
harris::harris(Mat img):
    src(img)
{

    Mat img2 = imread("2.jpg");
    Mat harris;

    Ptr<FeatureDetector> harris_detect = GFTTDetector::create(1000,0.01,10,3,true);
    vector<KeyPoint> keypoints1;
    vector<KeyPoint> keypoints2;
    Mat descriptors_object;
    Mat descriptors_scene;

    harris_detect->detect(src, keypoints1);
    harris_detect->detect(img2, keypoints2);

    Ptr<HARRIS> detector = HARRIS::create(400);
    detector->compute(src, keypoints1, descriptors_object);
    detector->compute(img2, keypoints2, descriptors_scene);

    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );

    double max_dist = 0; double min_dist = 100;

    for( int i = 0; i < descriptors_object.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }


    vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++ )
    { if( matches[i].distance < 3*min_dist )
        { good_matches.push_back( matches[i]); }
    }

    qDebug()<<"Good: "<< good_matches.size();

}
