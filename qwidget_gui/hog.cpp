#include "hog.h"

hog::hog(int arg1)
{
    threshold_value = arg1;

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


    //qDebug()<<"Keypints 1:" << keypoints_object.size();
    //qDebug()<<"Keypints 2:" << keypoints_scene.size();

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(keypoints_object);
    Mat descriptors_scene = computeKeypoints(keypoints_scene);

    double distance=0;
    double max_dist = 0; double min_dist = 100;

    for(int i=0;i<descriptors_object.rows;i++)
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
    }


}





