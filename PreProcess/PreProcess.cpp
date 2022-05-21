//
// Created by wjy on 22-5-21.
//

#include <iostream>
#include "PreProcess.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

Mat cameraMatrix = (Mat_<float>(3, 3) <<
        638.9910, 0, 320.4412,
        0, 639.3573, 254.3665,
        0, 0, 1);
Mat disCoeffs = (Mat_<float>(1, 4) <<
        -0.5024, 0.2813, 0, 0);
int wl[2] = {720, 1280};

void PreProcess(Mat& SrcImg){
    undistort(SrcImg, SrcImg, cameraMatrix, disCoeffs);
    Mat gray, binary;
    //图像的预处理
    cvtColor(SrcImg, gray, COLOR_BGR2GRAY);

    GaussianBlur(gray,gray,Size(5,5),10,20);
    threshold(gray, binary, 100, 255, THRESH_BINARY);

    Mat kernel = getStructuringElement(0, Size(2, 2));
    erode(binary, binary, kernel, Point(-1, -1), 3);
};
