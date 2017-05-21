#include "stitcher.h"

Stitcher::Stitcher(int arg1)
{
    threshold_value = arg1;


}

Mat Stitcher::alpha(Mat img){
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

Mat Stitcher::translateImage(Mat &img, Mat dst_size, int offset_x, int offset_y){
    Mat trans_mat = (Mat_<double>(2,3) << 1,0, offset_x, 0, 1, offset_y);
    warpAffine(img, img, trans_mat, dst_size.size());

    return trans_mat;
}

vector<KeyPoint> Stitcher::detectKeypoints(Mat image){
    Ptr<SURF> detector = SURF::create(threshold_value);
    vector< KeyPoint > keypoints;
    detector->detect(image, keypoints);
    return keypoints;
}

Mat Stitcher::computeKeypoints(Mat image, vector<KeyPoint> keypoints){
    Ptr<SURF> detector = SURF::create(threshold_value);
    Mat descriptors;
    detector->compute(image, keypoints, descriptors);
    return descriptors;
}

Mat Stitcher::reverseComparison(Mat image1, Mat image2, Mat refImage1, Mat refImage2){
    //-- Step 1: Detect the keypoints using SURF Detector
    vector< KeyPoint > keypoints_object = detectKeypoints(image1);
    vector< KeyPoint > keypoints_scene = detectKeypoints(image2);

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(image1, keypoints_object);
    Mat descriptors_scene = computeKeypoints(image2, keypoints_scene);

    //qDebug()<<"Keypoints object: "<<keypoints_object.size();
    //qDebug()<<"Keybpoints scene: "<<keypoints_scene.size();

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );

    //qDebug()<< "Matches: "<<matches.size();
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

    //qDebug()<<"Good matches: "<<good_matches.size();

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

void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;

            if (fX >= overlay->cols)
                break;

            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}
Mat Stitcher::startComparingRows(Mat image1, Mat image2, Mat refImage1, Mat refImage2){



    //qDebug() << threshold_value;
    //-- Step 1: Detect the keypoints using SURF Detector
    vector< KeyPoint > keypoints_object = detectKeypoints(image1);
    vector< KeyPoint > keypoints_scene = detectKeypoints(image2);

    Mat out1;
    drawKeypoints(image1, keypoints_object, out1);
    Mat out2;
    drawKeypoints(image1, keypoints_scene, out2);

    //qDebug()<<"Keypints 1:" << keypoints_object.size();
    //qDebug()<<"Keypints 2:" << keypoints_scene.size();

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_object = computeKeypoints(image1, keypoints_object);
    Mat descriptors_scene = computeKeypoints(image2, keypoints_scene);

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );

    //qDebug()<<"Matches"<<matches.size();
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
    { if( matches[i].distance <= 3*min_dist)
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
    qDebug()<<"Good matches:"<<good_matches.size();
    // Find the Homography Matrix
    Mat H = findHomography( obj, scene, CV_RANSAC );

    //cout << "vectorCompare = "<<endl<<" "<<H<<endl<<endl;
    //double det = H.at<double>(0,0)* H.at<double>(1,1) - H.at<double>(1,0)*H.at<double>(0,1);
    //cout << "det = "<< det <<endl;

    //qDebug()<<H.at<double>(0,2);
    //qDebug()<<H.at<double>(1,2);
    //qDebug()<<"";
    // Use the Homography Matrix to warp the images
    Mat result;
    Mat H2 ;

    if(abs(H.at<double>(0,2))> abs(H.at<double>(1,2)))
        if(H.at<double>(0,2)>0){
            warpPerspective(refImage1,result,H,cv::Size(refImage1.cols+refImage1.cols,refImage1.rows));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));

            refImage2 = alpha(refImage2);
            overlayImage(&half,&refImage2,Point());
            imshow("result",result);

            /*
            qDebug()<<"Keypoints object: "<<keypoints_object.size();
            qDebug()<<"Keybpoints scene: "<<keypoints_scene.size();
            qDebug()<< "Matches: "<<matches.size();
            qDebug()<<"Good matches: "<<good_matches.size();
            */
            qDebug()<<"1+2";
        }
        else{
            H2 = reverseComparison(image2, image1, refImage1, refImage2);
            warpPerspective(refImage2,result,H2,cv::Size(refImage1.cols+refImage2.cols,refImage1.rows));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage1.copyTo(half);
            qDebug()<<"2+1";
        }

    else{
        if(H.at<double>(1,2)>0){
            warpPerspective(refImage1,result,H,cv::Size(refImage1.cols,refImage1.rows+refImage2.rows));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage2.copyTo(half);
            /*
            qDebug()<<"Keypoints object: "<<keypoints_object.size();
            qDebug()<<"Keybpoints scene: "<<keypoints_scene.size();
            qDebug()<< "Matches: "<<matches.size();
            qDebug()<<"Good matches: "<<good_matches.size();
            */
            qDebug()<<"1/2";
        }
        else{
            H2 = reverseComparison(image2, image1,refImage1,refImage2);
            warpPerspective(refImage2,result,H2,cv::Size(refImage1.cols,refImage1.rows+refImage2.rows));
            cv::Mat half(result,cv::Rect(0,0,refImage2.cols,refImage2.rows));
            refImage1.copyTo(half);
            qDebug()<<"2/1";
        }

    }
    return result;
}
