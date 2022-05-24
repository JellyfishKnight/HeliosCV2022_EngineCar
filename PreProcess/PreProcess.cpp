//
// Created by wjy on 22-5-21.
//

#include "PreProcess.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;


void PreProcess(Mat &SrcImg, Mat &gray, Mat &binary, enum Plan plan) {
    Mat tempFrame;
    Mat cameraMatrix = (Mat_<float>(3, 3) << 638.9910, 0, 320.4412,
            0, 639.3573, 254.3665,
            0, 0, 1);
    Mat disCoeffs = (Mat_<float>(1, 4) << -0.5024, 0.2813, 0, 0);
    //去畸变
    undistort(SrcImg, tempFrame, cameraMatrix, disCoeffs);
    //图像的预处理
    SrcImg = tempFrame.clone();
    cvtColor(SrcImg, gray, COLOR_BGR2GRAY);

    GaussianBlur(gray, gray, Size(5, 5), 10, 20);
    if (plan == PlanA) {
        //threshold(gray, binary, 100, 255, THRESH_BINARY);
        adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 141, 20);
        Mat kernel = getStructuringElement(0, Size(2, 2));
        erode(binary, binary, kernel, Point(-1, -1), 3);
    } else if (plan == PlanB) {
        threshold(gray, binary, 100, 255, THRESH_OTSU != 0);
        //adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_OTSU != 0, 121, 20);
        Mat kernel_mor = getStructuringElement(MORPH_RECT, Size(1, 1));
        Mat kernel_dilate = getStructuringElement(MORPH_DILATE, Size(3, 3));
        morphologyEx(binary, binary, MORPH_OPEN, kernel_mor, Point(-1, -1), 1);
        dilate(binary, binary, kernel_dilate, Point(-1, -1), 4);
    }

}
