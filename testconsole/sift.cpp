#include "sift.h"
#include <QDebug>
sift::sift()
{
    Mat img1 = imread("1.jpg");
    Mat img2 = imread("2.jpg");

    Ptr<SIFT> detector = SIFT::create(400);

    vector<KeyPoint> keypoints1;
    vector<KeyPoint> keypoints2;
    Mat descriptors_object;
    Mat descriptors_scene;

    detector->detect(img1, keypoints1);
    detector->detect(img2, keypoints2);

    detector->compute(img1, keypoints1, descriptors_object);
    detector->compute(img2, keypoints2, descriptors_scene);

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );

    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_object.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    //printf("-- Max dist : %f \n", max_dist );
    //printf("-- Min dist : %f \n", min_dist );

    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
    vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++ )
    { if( matches[i].distance < 3*min_dist )
        { good_matches.push_back( matches[i]); }
    }

    qDebug()<<"Good: "<< good_matches.size();


}
