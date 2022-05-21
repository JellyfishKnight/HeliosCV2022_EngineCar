//
// Created by wjy on 22-5-21.
//

#ifndef HELIOSCV2022_ENGINETRUCK_DETECTOR_H
#define HELIOSCV2022_ENGINETRUCK_DETECTOR_H

#include<iostream>
#include<opencv2/core.hpp>


using namespace std;
using namespace cv;

/**
 * @brief主要介绍本类的一些数据成员
 * @param img->代表读入的原图
 * @param gray->处理后的灰度图
 * @param binary->处理后的二值化图
 * @param type->识别到的面的类型，1,2,3，分别代表空白，二维码，R面
 * @param up_dowm->上下旋转的角度，上为正
 * @param left_right->左右旋转的角度，左为正
 * @param wl->读入图像的长宽？
 */
class DetectorA{             //Plan A
                             //written by chenzhen
private:

    Mat img;
    Mat gray;
    Mat binary;
    int type = 0;
    float up_down = 2;
    float left_right = 2;
    int wl[2] = {720, 1280};

    void polygonDetect(Mat &srcImg, double epsilon = 9, int minAcreage = 0, int maxAcreage = 100000);

    static float ratio_wl(RotatedRect rrect);//计算外接矩形的长宽比

    static float cal_cos(float x1,float x2,float x3,float x4);   //the cost of vector<x1,x2>and<x3,x4>

    static bool rect_slope(vector<Point2f> Points);              //针对正方形

    static bool remove_parallelogram(vector<Point2f> Points);    //排除平行四边形

    void draw_contours(RotatedRect rrect);                //画出外接矩形

    static float squareRoot(float& d1,float& d2,float& d3,float& d4);//求4个数的标准差

    static float average(float& a1,float& a2,float& a3,float& a4);//求4个数的均值

    static bool target_test(vector<Point2f> Points);              //目标正方形边长的标准差是否小于max_squareRoot

    static int getcross(Point2f a,Point2f b,Point2f p);

    static bool isin(vector<Point2f> Points,Point2f p);           //点p是否在点集所围成的多边形内部

    static int rectangel(int form[],int p1,int p2,int p3,int p4);

    static int which_kind(vector<Point2f> contours_center,const vector<int>& tar_more,const vector<Point2f>& Points);

    void my_rotate(int inputType, vector<Point2f> Points, vector<Point2f> contours_center, const int form[], int p1, int p2, int p3, int p4);

    static Point2f aver_center(const vector<Point2f>&P);

public:
    explicit DetectorA(Mat& frame);

    Mat getImg();

    void detect();

    ~DetectorA();
};

#endif //HELIOSCV2022_ENGINETRUCK_DETECTOR_H
