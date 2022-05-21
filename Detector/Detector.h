//
// Created by wjy on 22-5-21.
//

#ifndef HELIOSCV2022_ENGINETRUCK_DETECTOR_H
#define HELIOSCV2022_ENGINETRUCK_DETECTOR_H

#include<iostream>
#include<opencv2/core.hpp>


using namespace std;
using namespace cv;

class DetectorA{             //Plan A
                             //written by chenzhen
private:
    Mat img;

    void polygonDetect(Mat &srcImg, double epsilon, int minAcreage, int maxAcreage);

    float ratio_wl(RotatedRect rrect);//计算外接矩形的长宽比

    float cal_cos(float x1,float x2,float x3,float x4);   //the cost of vector<x1,x2>and<x3,x4>

    bool rect_slope(vector<Point2f> Points);              //针对正方形

    bool remove_parallelogram(vector<Point2f> Points);    //排除平行四边形

    void draw_contours(RotatedRect rrect);                //画出外接矩形

    float squareRoot(float& d1,float& d2,float& d3,float& d4);//求4个数的标准差

    float average(float& a1,float& a2,float& a3,float& a4);//求4个数的均值

    bool target_test(vector<Point2f> Points);              //目标正方形边长的标准差是否小于max_squareRoot

    int getcross(Point2f a,Point2f b,Point2f p);

    bool isin(vector<Point2f> Points,Point2f p);           //点p是否在点集所围成的多边形内部

    int rectangel(int form[],int p1,int p2,int p3,int p4);

    int which_kind(vector<Point2f> contours_center,vector<int> tar_more,vector<Point2f> Points);

    void my_rotate(int type,vector<Point2f> Points,vector<Point2f> contours_center,int form[],int p1,int p2,int p3,int p4);

    Point2f aver_center(vector<Point2f>P);

public:
    explicit DetectorA(Mat& frame);

    Mat getImg();

    void detect();

    ~DetectorA();
};

#endif //HELIOSCV2022_ENGINETRUCK_DETECTOR_H
