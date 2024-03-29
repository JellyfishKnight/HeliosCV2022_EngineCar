//
// Created by wjy on 22-5-21.
//

#ifndef HELIOSCV2022_ENGINETRUCK_PREPROCESS_H
#define HELIOSCV2022_ENGINETRUCK_PREPROCESS_H

#include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

enum Plan {
    PlanA = 0,
    PlanB = 1
};

using namespace cv;

void PreProcess(Mat &SrcImg, Mat &gray, Mat &binary, enum Plan plan);


#endif //HELIOSCV2022_ENGINETRUCK_PREPROCESS_H
