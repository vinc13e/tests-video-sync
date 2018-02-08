#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

#define OUTPATH "/tmp/"

namespace fs = std::experimental::filesystem;
using namespace std;
using namespace cv;
using namespace fs;


void sincronize(string rgb, string tir);
long long findClosest(long long ts, std::vector<long long> & set);
void saveFrame(long long ts, std::vector<long long> & set, VideoWriter &vw, string & in_path, string name);

int main()
{
    std::string rgb_path = "/home/vmachado/Videos/polarys/clips/v1_1/rgb/";
    std::string tir_path = "/home/vmachado/Videos/polarys/clips/v1_1/tir/";
    sincronize(rgb_path, tir_path);
}


void saveFrame(long long ts, std::vector<long long> & set, VideoWriter &vw, string & in_path, string name){
    //TODO to_string
    char aaa[4];
    sprintf(aaa, "%03d", ts);
    Mat frame = imread(in_path+aaa+".jpg");
    //TODO if !frame ....
   // cv::imshow("frame", frame); waitKey();
    if(!vw.isOpened()){
        vw.open(OUTPATH+name+".mp4", CV_FOURCC('D','I','V','X'),25, Size(frame.cols,frame.rows),true);
    }
    vw << frame;
}

long long findClosest(long long ts, std::vector<long long> & set){
    auto closest = 0ll;
    auto diff = abs(ts - closest);
    for(auto s : set){
        if(abs(s-ts)<diff){
            closest = s;
            diff = abs(s-ts);
            continue;
        }
        break;
    }
    return closest;
}



void sincronize(string rgb, string tir){

    //sorted lists of timestamps
    std::vector<long long> rgb_tss, tir_tss;

    for (auto & p : fs::directory_iterator(rgb)) {
        fs::path fp(p);
        long long ll = std::stoll(fp.stem());
        rgb_tss.push_back(std::stoll(fp.stem()));

    }
    for (auto & p : fs::directory_iterator(tir)) {
        fs::path fp(p);
        long long ll = std::stoll(fp.stem());
        tir_tss.push_back(std::stoll(fp.stem()));
    }

    std::sort(rgb_tss.begin(), rgb_tss.end(), [](long long a, long long b) { return a < b; });
    std::sort(tir_tss.begin(), tir_tss.end(), [](long long a, long long b) { return a < b; });

    VideoWriter rgb_out, tir_out;

    bool isRGB=false;

    if(rgb_tss.size() < tir_tss.size()){
        isRGB=true;
        cout << "RGB ---" << endl;
    }
    else {
        isRGB=false;
        cout << "TIR ---" << endl;

    }

    bool initialized=false;

    auto & active = (isRGB) ? rgb_tss : tir_tss;
    auto & inactive = (isRGB) ? tir_tss : rgb_tss;
    auto & vw_ative = (isRGB) ? rgb_out : tir_out;
    auto & vw_inactive = (isRGB) ? tir_out : rgb_out;
    auto & active_is_path = (isRGB) ? rgb : tir;
    auto & inactive_is_path = (isRGB) ? tir : rgb;



    for(auto _ts : active){
        if(!initialized){
            if(_ts < inactive[0]) continue;
            initialized=true;
        }
        auto ts_inactive = findClosest(_ts, inactive);
        saveFrame(_ts, active, vw_ative, active_is_path, "video_a");
        saveFrame(ts_inactive, inactive, vw_inactive, inactive_is_path, "video_b");
        cout << _ts << " + " << ts_inactive << endl;
    }
}






//void readme();
///** @function main */
//int __main( int argc, char** argv )
//{
//    if( argc != 3 )
//    { readme(); return -1; }
//
//    Mat img_1 = imread( argv[1], IMREAD_GRAYSCALE );
//    Mat img_2 = imread( argv[2], IMREAD_GRAYSCALE );
//
//    if( !img_1.data || !img_2.data )
//    { std::cout<< " --(!) Error reading images " << std::endl; return -1; }
//
////
////    cv::resize(img_2,img_2,cv::Size(0,0), 1.05, 1.05);
////    Mat roi(img_1, Rect(0.97*(img_1.cols-img_2.cols)/2 ,1.55*(img_1.rows-img_2.rows)/2, img_2.cols, img_2.rows));
////    roi.copyTo(img_1);
//
//
//
//    //-- Step 1: Detect the keypoints using SURF Detector
//    int minHessian = 400;
//
//    Ptr<cv::ORB> detector = ORB::create( minHessian );
//
//    std::vector<KeyPoint> keypoints_1, keypoints_2;
//    cv::Mat desc1, desc2;
//    detector->detectAndCompute( img_1, Mat(), keypoints_1, desc1);
//    detector->detectAndCompute( img_2, Mat(), keypoints_2, desc2);
//
//    BFMatcher bf(cv::NORM_HAMMING, true);
//    std::vector<DMatch> matches, best6;
//    bf.match(desc1, desc2, matches);
//
//    DMatch d;
//    std::sort(matches.begin(), matches.end(), [](cv::DMatch a, cv::DMatch b) { return a.distance < b.distance; });
//    best6 = std::vector<DMatch>(matches.begin(), matches.begin()+6);
//
//
//    Mat out_img;
//    cv::drawMatches(img_1,keypoints_1,img_2, keypoints_2, best6, out_img);
//    imshow("out", out_img);
//
//    //-- Draw keypoints
//    Mat img_keypoints_1; Mat img_keypoints_2;
//
//    drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
//    drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
//
//    //-- Show detected (drawn) keypoints
//    imshow("Keypoints 1", img_keypoints_1 );
//    imshow("Keypoints 2", img_keypoints_2 );
//
//    waitKey(0);
//
//    return 0;
//}
//
///** @function readme */
//void readme()
//{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }
