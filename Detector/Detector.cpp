//
// Created by wjy on 22-5-21.
//
#include "Detector.h"
#include "../PreProcess/PreProcess.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

DetectorA::DetectorA(Mat &frame) : img(frame) {}

DetectorA::~DetectorA() = default;

Mat DetectorA::getImg() {
    return img;
}

void DetectorA::detect() {
    PreProcess(img);

}

