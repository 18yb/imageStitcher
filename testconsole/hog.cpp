#include "hog.h"
#include <QDebug>

hog::hog()
{

    HOGDescriptor *hog= new HOGDescriptor();;

    Mat img1 = imread("1.jpg");
    Mat img2 = imread("2.jpg");

    vector<float> keypoints1;
    vector<float> keypoints2;
    Mat descriptors_object;
    Mat descriptors_scene;
    vector<Point> locations;

    hog->compute(img1, keypoints1, Size(32,32), Size(0,0),locations);
    hog->compute(img2, keypoints2, Size(32,32), Size(0,0),locations);

    descriptors_object.create(keypoints1.size(),1,CV_32FC1);
    for(int i=0; i<keypoints1.size(); i++)
       descriptors_object.at<float>(i,0)=keypoints1.at(i);

    descriptors_scene.create(keypoints2.size(),1,CV_32FC1);
    for(int i=0; i<keypoints2.size(); i++)
       descriptors_scene.at<float>(i,0)=keypoints2.at(i);

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

    qDebug()<< "Good match: "<<counter;


}
