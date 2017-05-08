#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"



#include <roberts.h>
#include <sobel.h>
#include <laplacian.h>
#include <laplaciangaussian.h>


#define THRESHOLD 400
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

Mat translateImage(Mat &img, Mat dst_size, int offset_x, int offset_y){
    Mat trans_mat = (Mat_<double>(2,3) << 1,0, offset_x, 0, 1, offset_y);
    warpAffine(img, img, trans_mat, dst_size.size());

    return trans_mat;
}

vector<KeyPoint> detectKeypoints(Mat image){
    Ptr<SURF> detector = SURF::create(THRESHOLD);
    vector< KeyPoint > keypoints;
    detector->detect(image, keypoints);
    return keypoints;
}

Mat computeKeypoints(Mat image, vector<KeyPoint> keypoints){
    Ptr<SURF> detector = SURF::create(THRESHOLD);
    Mat descriptors;
    detector->compute(image, keypoints, descriptors);
    return descriptors;
}


Mat reverseComparison(Mat image1, Mat image2){
    //-- Step 1: Detect the keypoints using SURF Detector
    vector< KeyPoint > keypoints_object = detectKeypoints(image1);
    vector< KeyPoint > keypoints_scene = detectKeypoints(image2);

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(image1, keypoints_object);
    Mat descriptors_scene = computeKeypoints(image2, keypoints_scene);

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

    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );

    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
    vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++ )
    { if( matches[i].distance < 3*min_dist )
        { good_matches.push_back( matches[i]); }
    }
    vector< Point2f > obj;
    vector< Point2f > scene;

    for( int i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    // Find the Homography Matrix
    Mat H = findHomography( obj, scene, CV_RANSAC );

    cout << "vectorCompare = "<<endl<<" "<<H<<endl<<endl;
    double det = H.at<double>(0,0)* H.at<double>(1,1) - H.at<double>(1,0)*H.at<double>(0,1);
    cout << "det = "<< det <<endl;

    qDebug()<<H.at<double>(0,2);
    qDebug()<<H.at<double>(1,2);

    return H;
}

Mat startComparingRows(Mat image1, Mat image2){

    //-- Step 1: Detect the keypoints using SURF Detector
    vector< KeyPoint > keypoints_object = detectKeypoints(image1);
    vector< KeyPoint > keypoints_scene = detectKeypoints(image2);

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(image1, keypoints_object);
    Mat descriptors_scene = computeKeypoints(image2, keypoints_scene);

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

    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );

    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
    vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++ )
    { if( matches[i].distance < 3*min_dist )
        { good_matches.push_back( matches[i]); }
    }
    vector< Point2f > obj;
    vector< Point2f > scene;

    for( int i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    // Find the Homography Matrix
    Mat H = findHomography( obj, scene, CV_RANSAC );

    cout << "vectorCompare = "<<endl<<" "<<H<<endl<<endl;
    double det = H.at<double>(0,0)* H.at<double>(1,1) - H.at<double>(1,0)*H.at<double>(0,1);
    cout << "det = "<< det <<endl;

    qDebug()<<H.at<double>(0,2);
    qDebug()<<H.at<double>(1,2);

    // Use the Homography Matrix to warp the images
    cv::Mat result;
    Mat H2 ;

    if(H.at<double>(0,2) > H.at<double>(1,2))
        if(H.at<double>(0,2)>0){
            warpPerspective(image1,result,H,cv::Size(image1.cols+int(H.at<double>(0,2)),image1.rows));
            cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
            image2.copyTo(half);


        }
        else{
            H2 = reverseComparison(image2, image1);
            warpPerspective(image2,result,H2,cv::Size(image1.cols,image1.rows+abs(int(H.at<double>(1,2)))));
            cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
            image1.copyTo(half);

        }
    else{
        if(H.at<double>(0,2)>0){
            warpPerspective(image1,result,H,cv::Size(image1.cols,image1.rows+int(H.at<double>(1,2))));
            cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
            image2.copyTo(half);
        }
        else{
            H2 = reverseComparison(image2, image1);
            warpPerspective(image2,result,H2,cv::Size(image1.cols+abs(int(H2.at<double>(0,2))),image1.rows));
            cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
            image1.copyTo(half);
        }
    }


    imshow( "Result", result );

    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if( argc != 3 )
    {  return -1; }

    // Load the images
    /*Mat image1= imread("p1.jpg", 1 );
    Mat image2= imread("p2.jpg", 1 );
    Mat gray_image1;
    Mat gray_image2;
    cvtColor( image1, gray_image1, CV_RGB2GRAY );
    cvtColor( image2, gray_image2, CV_RGB2GRAY );*/

    Mat img1 = imread("p1.jpg", 1 );
    Mat img2 = imread("p2.jpg", 1 );


    roberts *method_11 = new roberts(img1);
    sobel *method_12 = new sobel(img1);
    laplacian *method_13 = new laplacian(img1);
    laplaciangaussian *method_14 = new laplaciangaussian(img1);

    roberts *method_21 = new roberts(img2);
    sobel *method_22 = new sobel(img2);
    laplacian *method_23 = new laplacian(img2);
    laplaciangaussian *method_24 = new laplaciangaussian(img2);

    Mat image1 = method_13->img;
    Mat image2 = method_23->img;

    Mat vectorCompare = startComparingRows(image1,image2);


    return a.exec();
}
