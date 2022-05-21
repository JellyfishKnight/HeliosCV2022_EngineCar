//
// Created by wjy on 22-5-21.
//

#ifndef HELIOSCV2022_ENGINETRUCK_PREPROCESS_H
#define HELIOSCV2022_ENGINETRUCK_PREPROCESS_H

#include<opencv2/core.hpp>

using namespace cv;

extern Mat cameraMatrix;
extern Mat disCoeffs;
extern int wl[2];

void PreProcess(Mat& SrcImg);

#endif //HELIOSCV2022_ENGINETRUCK_PREPROCESS_H
