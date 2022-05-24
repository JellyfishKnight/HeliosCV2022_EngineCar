//
// Created by wjy on 22-5-21.
//

#ifndef HELIOSCV2022_ENGINETRUCK_DETECTOR_H
#define HELIOSCV2022_ENGINETRUCK_DETECTOR_H

#include<iostream>
#include<opencv2/core.hpp>
#include "SerialPort.h"

using namespace std;
using namespace cv;


/**
 * @brief 需要被传输的数据
 * @param set->自由度,0为左右自由度,1为上下自由度
 * @param direction->1为左或上,0为右或下,2为向下翻两次
 * @param toggle_flag->-1为旋转,0为停止
 */
extern int set;
extern int direction;
extern int toggle_flag;


/**
 * @brief主要介绍本类的一些数据成员
 * @param img->代表读入的原图
 * @param gray->处理后的灰度图
 * @param binary->处理后的二值化图
 * @param type->识别到的面的类型，1,2,3，分别代表空白，二维码，R面
 * @param up_down->上下旋转的角度，上为正
 * @param left_right->左右旋转的角度，左为正
 * @param center_rotate->中心旋转的角度，逆时针为正
 * @param wl->读入图像的长宽？
 */
class DetectorA {             //Plan A
    //written by chenzhen
private:

    Mat img;
    Mat gray;
    Mat binary;
    int type = 0;
    float up_down = 2;
    float left_right = 2;
    float center_rotate = 0;//中心旋转的角度，逆时针为正
    int wl[2] = {720, 1280};

    void polygonDetect(Mat &srcImg, double epsilon = 9, int minAcreage = 0, int maxAcreage = 100000);

    static float ratio_wl(RotatedRect rrect);//计算外接矩形的长宽比

    static float cal_cos(float x1, float x2, float x3, float x4);   //the cost of vector<x1,x2>and<x3,x4>

    static bool rect_slope(vector<Point2f> Points);              //针对正方形

    static bool remove_parallelogram(vector<Point2f> Points);    //排除平行四边形

    void draw_contours(RotatedRect rrect);                //画出外接矩形

    static float squareRoot(float &d1, float &d2, float &d3, float &d4);//求4个数的标准差

    static float average(float &a1, float &a2, float &a3, float &a4);//求4个数的均值

    static bool target_test(vector<Point2f> Points);              //目标正方形边长的标准差是否小于max_squareRoot

    static int getcross(Point2f a, Point2f b, Point2f p);

    static bool isin(vector<Point2f> Points, Point2f p);           //点p是否在点集所围成的多边形内部

    static int rectangel(int form[], int p1, int p2, int p3, int p4);

    static int which_kind(vector<Point2f> contours_center, const vector<int> &tar_more, const vector<Point2f> &Points);

    void
    my_rotate(int inputType, vector<Point2f> Points, vector<Point2f> contours_center, const int form[], int p1, int p2,
              int p3, int p4);

    static Point2f aver_center(const vector<Point2f> &P);

public:
    explicit DetectorA(Mat &frame);

    Mat getImg();

    void detect();

    void SetData() const;

    ~DetectorA();
};


#define Up_1 0
#define Left_1 1
#define Right_1 2
#define Down_1 3
#define Down_2 4
#define NoMove 5

/**
 * @brief 对本类的数据成员的解释
 * @param img->原图
 * @param gray->处理后的灰度图
 * @param binary->处理后的二值化图
 * @param Direction->应该旋转的方向
 * @param CenterOfCenter->矿石的正中心
 * @param contours->第一次寻找到的所有轮廓
 * @param rects->筛选掉小噪点的矩形拟合
 * @param interested_contours->在限制条件下匹配成功的矩形
 * @param cornerRect->四个角的可能矩形
 * @param cornerContours->四个角的可能矩形的轮廓
 * @param point2F->在角上的矩形的四个角点坐标
 * @param FullFilledRects->被填充满的矩形
 * @param CenterOfRects->在角上的矩形的中心点
 * @param number->标记被填充满的矩形的下标
 * @param contours_barcode->寻找条形码中的轮廓
 */
class DetectorB {
private:

    Mat img;
    Mat gray;
    Mat binary;
    int Direction{};

    Point CenterOfCenter;

    vector<vector<Point>> contours;
    vector<RotatedRect> rects;

    vector<vector<Point>> interested_contours;
    vector<vector<Point>> cornerContours;
    vector<RotatedRect> cornerRect;

    Point2f point2F[4][4];
    vector<RotatedRect> FullFiilledRects;
    vector<Point> CentersOfRects;
    vector<int> number;
    int num{};

    vector<vector<Point>> contours_barcode;

    void SelectContours();

    bool SelectCornerRect();

    void findFullFilledRect();

    void SolveCenter();

    bool findBarCode();

    int JudgeSides();

public:
    DetectorB(Mat &frame);

    Mat getImg();

    void detect();

    void SetData() const;

    ~DetectorB();

};

#endif //HELIOSCV2022_ENGINETRUCK_DETECTOR_H
