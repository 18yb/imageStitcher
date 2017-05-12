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

#include <QDir>


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


Mat reverseComparison(Mat image1, Mat image2, Mat refImage1, Mat refImage2){
    //-- Step 1: Detect the keypoints using SURF Detector
    vector< KeyPoint > keypoints_object = detectKeypoints(image1);
    vector< KeyPoint > keypoints_scene = detectKeypoints(image2);

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(image1, keypoints_object);
    Mat descriptors_scene = computeKeypoints(image2, keypoints_scene);

    qDebug()<<"Keypoints object: "<<keypoints_object.size();
    qDebug()<<"Keybpoints scene: "<<keypoints_scene.size();

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );

    qDebug()<< "Matches: "<<matches.size();
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

    qDebug()<<"Good matches: "<<good_matches.size();

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

    //cout << "vectorCompare = "<<endl<<" "<<H<<endl<<endl;
    double det = H.at<double>(0,0)* H.at<double>(1,1) - H.at<double>(1,0)*H.at<double>(0,1);
    //cout << "det = "<< det <<endl;

    //qDebug()<<H.at<double>(0,2);
    //qDebug()<<H.at<double>(1,2);

    return H;
}

Mat startComparingRows(Mat image1, Mat image2, Mat refImage1, Mat refImage2){

    //-- Step 1: Detect the keypoints using SURF Detector
    vector< KeyPoint > keypoints_object = detectKeypoints(image1);
    vector< KeyPoint > keypoints_scene = detectKeypoints(image2);

    Mat out;
    drawKeypoints(image1, keypoints_object, out);

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

    //cout << "vectorCompare = "<<endl<<" "<<H<<endl<<endl;
    double det = H.at<double>(0,0)* H.at<double>(1,1) - H.at<double>(1,0)*H.at<double>(0,1);
    //cout << "det = "<< det <<endl;

    //qDebug()<<H.at<double>(0,2);
    //qDebug()<<H.at<double>(1,2);
    qDebug()<<"";
    // Use the Homography Matrix to warp the images
    cv::Mat result;
    Mat H2 ;

    if(H.at<double>(0,2) > H.at<double>(1,2))
        if(H.at<double>(0,2)>0){
            warpPerspective(refImage1,result,H,cv::Size(refImage1.cols+int(H.at<double>(0,2)),refImage1.rows));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage2.copyTo(half);

            qDebug()<<"Keypoints object: "<<keypoints_object.size();
            qDebug()<<"Keybpoints scene: "<<keypoints_scene.size();
            qDebug()<< "Matches: "<<matches.size();
            qDebug()<<"Good matches: "<<good_matches.size();

        }
        else{
            H2 = reverseComparison(image2, image1,refImage1,refImage2);
            warpPerspective(refImage2,result,H2,cv::Size(refImage1.cols,refImage1.rows+abs(int(H.at<double>(1,2)))));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage1.copyTo(half);

        }
    else{
        if(H.at<double>(0,2)>0){
            warpPerspective(refImage1,result,H,cv::Size(refImage1.cols,refImage1.rows+int(H.at<double>(1,2))));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage2.copyTo(half);

            qDebug()<<"Keypoints object: "<<keypoints_object.size();
            qDebug()<<"Keybpoints scene: "<<keypoints_scene.size();
            qDebug()<< "Matches: "<<matches.size();
            qDebug()<<"Good matches: "<<good_matches.size();

        }
        else{
            H2 = reverseComparison(image2, image1, refImage1, refImage2);
            warpPerspective(refImage2,result,H2,cv::Size(refImage1.cols+abs(int(H2.at<double>(0,2))),refImage1.rows));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage1.copyTo(half);
        }
    }




    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Load the images
   /* Mat image1= imread("p32.jpg", 1 );
    Mat image2= imread("p31.jpg", 1 );
    Mat gray_image1;
    Mat gray_image2;
    clock_t begin = clock();


    cvtColor( image1, gray_image1, CV_RGB2GRAY );
    cvtColor( image2, gray_image2, CV_RGB2GRAY );

    */
    Mat img1 = imread("kan2.jpg", 1 );
    Mat img2 = imread("kan3blued.jpg", 1 );

    // -------------------------
    clock_t begin = clock();
    qDebug()<<"ROBERTS --------------";
    roberts *method_11 = new roberts(img1);
    roberts *method_21 = new roberts(img2);
    Mat image1 = method_11->img;
    Mat image2 = method_21->img;
    Mat vectorCompare = startComparingRows(image1,image2, img1, img2);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    qDebug()<<"Elapsed time: "<<elapsed_secs;

    // -------------------------
    qDebug()<<"SOBEL --------------";

    clock_t begin1 = clock();
    sobel *method_12 = new sobel(img1);
    sobel *method_22 = new sobel(img2);
    image1 = method_12->img;
    image2 = method_21->img;
    vectorCompare = startComparingRows(image1,image2, img1, img2);
    clock_t end1 = clock();
    elapsed_secs = double(end1 - begin1) / CLOCKS_PER_SEC;
    qDebug()<<"Elapsed time: "<<elapsed_secs;

    // -------------------------
    qDebug()<<"LAPLACE --------------";
    clock_t begin2 = clock();
    laplacian *method_13 = new laplacian(img1);
    laplacian *method_23 = new laplacian(img2);
    image1 = method_13->img;
    image2 = method_23->img;
    vectorCompare = startComparingRows(image1,image2, img1, img2);
    clock_t end2 = clock();
    elapsed_secs = double(end2 - begin2) / CLOCKS_PER_SEC;
    qDebug()<<"Elapsed time: "<<elapsed_secs;

    // -------------------------
    qDebug()<<"LOG --------------";
    clock_t begin3 = clock();
    laplaciangaussian *method_14 = new laplaciangaussian(img1);
    laplaciangaussian *method_24 = new laplaciangaussian(img2);
    image1 = method_14->img;
    image2 = method_24->img;
    Mat vectorCompare1 = startComparingRows(image1,image2, img1, img2);
    imshow( "Result1", vectorCompare1 );
    clock_t end3 = clock();
    elapsed_secs = double(end3 - begin3) / CLOCKS_PER_SEC;
    qDebug()<<"Elapsed time: "<<elapsed_secs;

    // -------------------------
    qDebug()<<"CLASSIC --------------";
    Mat gray_image1;
    Mat gray_image2;

    clock_t begin4 = clock();

    cvtColor( img1, gray_image1, CV_RGB2GRAY );
    cvtColor( img2, gray_image2, CV_RGB2GRAY );

    vectorCompare = startComparingRows(gray_image1,gray_image2,img1, img2);
    clock_t end4 = clock();
    elapsed_secs = double(end4 - begin4) / CLOCKS_PER_SEC;
    qDebug()<<"Elapsed time: "<<elapsed_secs;

    return a.exec();
}
