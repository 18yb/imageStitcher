#include "hog.h"
#include <QDebug>

hog::hog(int arg1)
{
    threshold_value = arg1;

}

Mat hog::alpha(Mat img){
    Mat image_bgra;

    cvtColor(img, image_bgra, CV_BGR2BGRA);

    for (int y = 0; y < image_bgra.rows; ++y){
        for (int x = 0; x < image_bgra.cols; ++x)
        {
            cv::Vec4b & pixel = image_bgra.at<cv::Vec4b>(y, x);
            if (pixel[0] <= 20 && pixel[1] <= 20 && pixel[2] <= 20)
            {
                pixel[3] = 0;
            }
        }
    }
    return image_bgra;
}

Mat hog::translateImage(Mat &img, Mat dst_size, int offset_x, int offset_y){
    Mat trans_mat = (Mat_<double>(2,3) << 1,0, offset_x, 0, 1, offset_y);
    warpAffine(img, img, trans_mat, dst_size.size());

    return trans_mat;
}

vector<float> hog::detectKeypoints(Mat image){
    HOGDescriptor *hog= new HOGDescriptor();
    vector< float > keypoints;
    vector<Point> locations;

    hog->compute(image, keypoints, Size(32,32), Size(0,0),locations);

    return keypoints;
}

Mat hog::computeKeypoints(vector<float> keypoints){
    Mat descriptors;

    descriptors.create(keypoints.size(),1,CV_32FC1);
    for(int i=0; i<keypoints.size(); i++)
       descriptors.at<float>(i,0)=keypoints.at(i);

    return descriptors;
}

Mat hog::startComparingRows(Mat image1, Mat image2, Mat refImage1, Mat refImage2){



    //qDebug() << threshold_value;
    //-- Step 1: Detect the keypoints using SURF Detector
    vector< float > keypoints_object = detectKeypoints(image1);
    vector< float > keypoints_scene = detectKeypoints(image2);


    qDebug()<<"Keypints 1:" << keypoints_object.size();
    qDebug()<<"Keypints 2:" << keypoints_scene.size();

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(keypoints_object);
    Mat descriptors_scene = computeKeypoints(keypoints_scene);

    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );

    qDebug()<<"Matches"<<matches.size();

    double distance=0;
    double max_dist = 0; double min_dist = 100;

    /*for(int i=0;i<descriptors_object.rows;i++)
    {
       distance = abs(descriptors_object.at<float>(i,0) - descriptors_scene.at<float>(i,0));
       if( distance < min_dist ) min_dist = distance;
       if( distance > max_dist ) max_dist = distance;

    }

    int counter = 0;

    for(int i=0;i<descriptors_object.rows;i++)
    {
       distance = abs(descriptors_object.at<float>(i,0) - descriptors_scene.at<float>(i,0));
       if( distance <= 3*min_dist)
           counter++;
    }*/


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

    qDebug()<<"Good matches: "<<good_matches.size();

    vector< Point2f > obj;
    vector< Point2f > scene;

    /*/for( int i = 0; i < good_matches.size(); i++ )
    {
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ] );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ] );
    }*/


}





