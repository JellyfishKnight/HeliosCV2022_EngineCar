//
// Created by wjy on 22-5-21.
//
#include "Detector.h"
#include "PreProcess.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int set;
int direction;
int toggle_flag;

using namespace std;
using namespace cv;

DetectorA::DetectorA(Mat &frame) : img(frame) {}

DetectorA::~DetectorA() = default;

Mat DetectorA::getImg() {
    return img;
}

void DetectorA::detect() {
    PreProcess(img, gray, binary, PlanA);
    polygonDetect(binary, 9, 0, 100000);
}

/**
 * @brief 识别的主流程
 * @param srcImg
 * @param epsilon
 * @param minAcreage
 * @param maxAcreage
 */
void DetectorA::polygonDetect(Mat &srcImg, double epsilon, int minAcreage, int maxAcreage) {

    //轮廓发现与绘制
    vector<vector<Point>> contours;//轮廓
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    vector<float> contours_realarea;//轮廓面积
    for (int t = 0; t < contours.size(); t++) {
        drawContours(srcImg, contours, t, Scalar(255, 0, 0), 2, 8);
        float area = contourArea(contours[t]);
        contours_realarea.push_back(area);
    }
    vector<float> contours_ratio_wl;//长宽比
    vector<float> contours_area;//外接矩形面积
    vector<Point2f> contours_center;//外接矩形中心
    int form[contours.size()];
    vector<Point2f> result;

    for (int t = 0; t < contours.size(); t++) {
        RotatedRect rrect = minAreaRect(contours[t]);//最小外接矩形

        draw_contours(rrect);
        float ratio = ratio_wl(rrect);
        contours_ratio_wl.push_back(ratio);//外接矩形的长宽比

        approxPolyDP(contours[t], result, 9, true);//求多边形的边数
        Point2f center = aver_center(result);
        //Point2f center=rrect.center;//外接矩形的中心
        contours_center.push_back(center);//将中心点保存
        float area = (rrect.size.height * rrect.size.width);//计算外接矩形的尺寸，以便筛选合适的轮廓
        contours_area.push_back(area);
        //circle(srcImg,center,2,Scalar(0,255,0),2,8,0);

        //approxPolyDP(contours[t],result,epsilon,true);//求多边形的边数
        if (result.size() == 6 && contours_realarea[t] > minAcreage && contours_realarea[t] < maxAcreage)//如果为6条边符合一类
        {
            putText(srcImg, "0", center, 0, 1, Scalar(0, 255, 0), 1, 8);
            // putText(srcImg,ratio,center,0,1,Scalar(0,255,0),1,8);
            form[t] = 0;
        } else if (result.size() == 4 && rect_slope(result) && contours_realarea[t] > minAcreage &&
                   contours_realarea[t] < maxAcreage)//如果为4条边符合另一类
        {

            putText(srcImg, "1", center, 0, 1, Scalar(0, 255, 0), 1, 8);
            form[t] = 1;
        } else {
            //putText(srcImg,"-1",center,0,1,Scalar(0,0,255),1,8);
            form[t] = -1;
        }
    }

    vector<int> tar_more;//包含中间二维码类型
    vector<int> tar;
    for (int i = 0; i < contours.size(); i++) {
        if (form[i] == 0 || form[i] == 1) {
            tar_more.push_back(i);
            if (contours_ratio_wl[i] < 1.5)
                tar.push_back(i);//保存需要的类型对应的轮廓序号
        }
    }

    float temp_dist;
    vector<float> dist;
    if (tar.size() >= 4) {
        for (int i: tar) {
            temp_dist = (contours_center[i].x - wl[0] / 2) * (contours_center[i].x - wl[0] / 2) +
                        (contours_center[i].y - wl[1] / 2) * (contours_center[i].y - wl[1] / 2);
            dist.push_back(temp_dist);
        }
        int min1, min2, min3, min4;//距离中心最近的4个点
        temp_dist = 1000000;
        for (int i = 0; i < dist.size(); i++) {
            if (dist[i] < temp_dist) {
                min1 = i;
                temp_dist = dist[i];
            }
        }
        circle(binary, contours_center[tar[min1]], 2, Scalar(0, 0, 0), 2, 8, 0);

        temp_dist = 1000000;
        for (int i = 0; i < dist.size(); i++) {
            if (dist[i] < temp_dist && i != min1) {
                min2 = i;
                temp_dist = dist[i];
            }
        }
        circle(binary, contours_center[tar[min2]], 2, Scalar(0, 0, 0), 2, 8, 0);

        temp_dist = 1000000;
        for (int i = 0; i < dist.size(); i++) {
            if (dist[i] < temp_dist && i != min1 && i != min2) {
                min3 = i;
                temp_dist = dist[i];
            }
        }
        circle(binary, contours_center[tar[min3]], 2, Scalar(0, 0, 0), 2, 8, 0);

        temp_dist = 1000000;
        for (int i = 0; i < dist.size(); i++) {
            if (dist[i] < temp_dist && i != min1 && i != min2 && i != min3) {
                min4 = i;
                temp_dist = dist[i];
            }
        }

        vector<Point2f> tar_Points;
        tar_Points.push_back(contours_center[tar[min1]]);
        tar_Points.push_back(contours_center[tar[min2]]);
        tar_Points.push_back(contours_center[tar[min3]]);
        tar_Points.push_back(contours_center[tar[min4]]);
        bool change = rect_slope(tar_Points);

        if (change) {
            if (remove_parallelogram(tar_Points)) {
                line(binary, contours_center[tar[min1]], contours_center[tar[min2]], 2, 8, 0);
                line(binary, contours_center[tar[min2]], contours_center[tar[min3]], 2, 8, 0);
                line(binary, contours_center[tar[min3]], contours_center[tar[min4]], 2, 8, 0);
                line(binary, contours_center[tar[min4]], contours_center[tar[min1]], 2, 8, 0);
                line(binary, contours_center[tar[min2]], contours_center[tar[min4]], 2, 8, 0);
                line(binary, contours_center[tar[min3]], contours_center[tar[min1]], 2, 8, 0);

                cout << "1:" << form[tar[min1]] << endl;
                cout << "2:" << form[tar[min2]] << endl;
                cout << "3:" << form[tar[min3]] << endl;
                cout << "4:" << form[tar[min4]] << endl;
                if (rectangel(form, tar[min1], tar[min2], tar[min3], tar[min4]) == 1)
                    type = 3;
                else type = which_kind(contours_center, tar_more, tar_Points);
            }
        } else type = 4;

        my_rotate(type, tar_Points, contours_center, form, tar[min1], tar[min2], tar[min3], tar[min4]);
    }
    circle(binary, Point2f(wl[0] / 2, wl[1] / 2), 2, Scalar(0, 0, 0), 2, 8, 0);
    imshow("binary", binary);
}


/**
 * @brief 计算外接矩形的长宽比
 * @param rrect
 * @return 长宽比数值
 */

float DetectorA::ratio_wl(RotatedRect rrect) {
    if (rrect.size.width > rrect.size.height)
        return rrect.size.width / rrect.size.height;
    else
        return rrect.size.height / rrect.size.width;
}


/**
 * @brief 解算两个向量的夹角余弦
 * @param x1
 * @param x2
 * @param x3
 * @param x4
 * @return 返回夹角余弦值
 */

float DetectorA::cal_cos(float x1, float x2, float x3, float x4) {
    return (x1 * x3 + x2 * x4) / sqrt(pow(x1, 2) + pow(x2, 2)) / sqrt(pow(x3, 2) + pow(x4, 2));
}

/**
 * @brief 函数作用
 * @param Points 角点坐标
 * @return 返回
 */
bool DetectorA::rect_slope(vector<Point2f> Points) {
    Point2f temp_Point;

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (Points[j].x < Points[i].x) {
                temp_Point = Points[i];
                Points[i] = Points[j];
                Points[j] = temp_Point;
            }
        }
    }

    float cos1 = cal_cos(Points[0].x - Points[1].x, Points[0].y - Points[1].y,
                         Points[2].x - Points[3].x, Points[2].y - Points[3].y);
    float cos2 = cal_cos(Points[0].x - Points[2].x, Points[0].y - Points[2].y,
                         Points[1].x - Points[3].x, Points[1].y - Points[3].y);


    if (abs(abs(cos1) - 1) < 0.02 && abs(abs(cos2) - 1) < 0.02)
        return true;
    else return false;
}

/**
 * @brief 判断是否为平行四边形
 * @param Points 角点坐标
 * @return 是为true，不是为false
 */

bool DetectorA::remove_parallelogram(vector<Point2f> Points) {
    float dist1 = sqrt(pow(Points[0].x - Points[2].x, 2) + pow(Points[0].y - Points[2].y, 2));
    float dist2 = sqrt(pow(Points[1].x - Points[3].x, 2) + pow(Points[1].y - Points[3].y, 2));
    if (abs(dist1 / dist2 - 1) < 0.3) return true;
    else return false;
}

/**
 * @brief 画出外接矩形
 * @param rrect
 */

void DetectorA::draw_contours(RotatedRect rrect) {
    Point2f points[4];
    rrect.points(points);
    for (int i = 0; i < 4; i++) {
        if (i == 3) {
            line(img, points[i], points[0], Scalar(0, 0, 255), 2, 8, 0);
            break;
        }
        line(img, points[i], points[i + 1], Scalar(0, 0, 255), 2, 8, 0);
    }
}

/**
 * @brief 求四个点之间的距离
 * @param Points 输入点坐标
 * @return
 */

bool DetectorA::target_test(vector<Point2f> Points) {
    float dis[4];
    dis[0] = sqrt(pow(Points[0].x - Points[1].x, 2) + pow(Points[0].y - Points[1].y, 2));
    dis[1] = sqrt(pow(Points[0].x - Points[2].x, 2) + pow(Points[0].y - Points[2].y, 2));
    dis[2] = sqrt(pow(Points[2].x - Points[3].x, 2) + pow(Points[2].y - Points[3].y, 2));
    dis[3] = sqrt(pow(Points[1].x - Points[3].x, 2) + pow(Points[1].y - Points[3].y, 2));

    cout << squareRoot(dis[0], dis[1], dis[2], dis[3]) << endl;
    return true;
}

/**
 * @brief 求平均距离
 * @param a1
 * @param a2
 * @param a3
 * @param a4
 * @return 距离值
 */
float DetectorA::average(float &a1, float &a2, float &a3, float &a4) {
    float ave;
    ave = (a1 + a2 + a3 + a4) / 4;
    return ave;

}

/**
 * @brief 求四个点距离的标准差
 * @param d1
 * @param d2
 * @param d3
 * @param d4
 * @return 标准差的值
 */

float DetectorA::squareRoot(float &d1, float &d2, float &d3, float &d4) {
    float b1, b2, b3, b4;
    float sr, aveg;
    aveg = average(b1, b2, b3, b4);
    sr = sqrt((b1 - aveg) * (b1 - aveg) + (b2 - aveg) * (b2 - aveg) + (b3 - aveg) * (b3 - aveg) +
              (b3 - aveg) * (b3 - aveg));
    return sr;
}

/**
 * @brief 求正方形个数
 * @param form
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 * @return 正方形个数
 */

int DetectorA::rectangel(int *form, int p1, int p2, int p3, int p4) {
    return form[p1] + form[p2] + form[p3] + form[p4];
}

/**
 * @brief 判断图案对应类型：空白或者是二维码
 * @param contours_center
 * @param tar_more
 * @param Points
 * @return type
 */

int DetectorA::which_kind(vector<Point2f> contours_center, const vector<int> &tar_more, const vector<Point2f> &Points) {
    int flag1 = 0, count1 = 0, flag2 = 0, count2 = 0, sum = 0;

    for (auto &i: contours_center)//全部点
    {
        if (isin(Points, i)) {
            flag1 = 1, count1++;
        }
    }
    if (count1 == 0) return 1;

    for (int i: tar_more)//二维码
    {
        if (isin(Points, contours_center[i])) {
            flag2 = 1, count2++;
        }
    }
    if (count2 > 0) return 2;
    else return 4;//表示错误
}


/**
 * @brief 判断点是否在Points之内
 * @param a
 * @param b
 * @param p
 * @return true or false
 */

int DetectorA::getcross(Point2f a, Point2f b, Point2f p) {
    return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
}


bool DetectorA::isin(vector<Point2f> Points, Point2f p) {
    float boo1 = getcross(Points[0], Points[1], p);
    float boo2 = getcross(Points[1], Points[3], p);
    float boo3 = getcross(Points[3], Points[2], p);
    float boo4 = getcross(Points[2], Points[1], p);
    if (boo1 > 0 && boo2 > 0 && boo3 > 0 && boo4 > 0 || boo1 < 0 && boo2 < 0 && boo3 < 0 && boo4 < 0) {
        return true;
    } else return false;
}


/**
 * 翻转情况
 * @param inputType
 * @param Points
 * @param contours_center
 * @param form
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 */
void
DetectorA::my_rotate(int inputType, vector<Point2f> Points, vector<Point2f> contours_center, const int *form, int p1,
                     int p2,
                     int p3, int p4) {
    if (inputType == 1) {
        up_down = 0;
        left_right = 0;
    }
    if (inputType == 2) {
        up_down = 2;
        left_right = 0;
    }
    if (inputType == 3) {
        int p = 0, flag = 0;
        float center_x = (Points[0].x + Points[1].x + Points[2].x + Points[3].x) / 4;
        float center_y = (Points[0].y + Points[1].y + Points[2].y + Points[3].y) / 4;

        if (form[p1] == 1) p = p1;
        if (form[p2] == 1) p = p2;
        if (form[p3] == 1) p = p3;
        if (form[p4] == 1) p = p4;

        if (contours_center[p].y > center_y && contours_center[p].x > center_x) flag = 4;
        if (contours_center[p].y > center_y && contours_center[p].x < center_x) flag = 3;
        if (contours_center[p].y < center_y && contours_center[p].x > center_x) flag = 2;
        if (contours_center[p].y < center_y && contours_center[p].x < center_x) flag = 1;

        /*float tany_x=(contours_center[form[p]].y-center_y)/(contours_center[form[p]].x-center_x);
        float atany_x=atan(tany_x);
        float degree=atany_x*180/3.1415926;*/

        if (flag == 4) {
            up_down = -1;
            left_right = 0;
        }
        if (flag == 3) {
            up_down = 0;
            left_right = 1;
        }
        if (flag == 2) {
            up_down = 0;
            left_right = -1;
        }
        if (flag == 1) {
            up_down = 1;
            left_right = 0;
        }
        cout << "flag<<" << flag << endl;
    }
    if (inputType == 4) {
        up_down = 2;
        left_right = 2;
    }
    cout << "======" << set << direction << toggle_flag << endl;
}

/**
 * @brief 解算中心点
 * @param P
 * @return 中心点坐标
 */

Point2f DetectorA::aver_center(const vector<Point2f> &P) {
    Point2f result;
    for (auto &i: P) {
        result.x += i.x;
        result.y += i.y;
    }
    result.x /= P.size();
    result.y /= P.size();
    return result;
}

void DetectorA::SetData() const {
    if (up_down != 0 && left_right == 0) {
        set = 1;
        toggle_flag = 1;
        if (up_down > 0) {
            direction = 1;
        } else {
            direction = 0;
        }
    } else if (up_down == 0 && left_right != 0) {
        set = 0;
        toggle_flag = 1;
        if (left_right > 0) {
            direction = 1;
        } else {
            direction = 0;
        }
    } else if (up_down == 0 && left_right == 0) {
        toggle_flag = 0;
    } else if (up_down == 2 && left_right == 2) {
        toggle_flag = 0;
    }
}


//Detection B

DetectorB::DetectorB(Mat &frame) : img(frame) {}

DetectorB::~DetectorB() = default;

Mat DetectorB::getImg() {
    return img;
}

void DetectorB::detect() {
    PreProcess(img, gray, binary, PlanB);
    SelectContours();
    if (SelectCornerRect()) {
        findFullFilledRect();
        SolveCenter();
        findBarCode();
        Direction = JudgeSides();
    } else {
        Direction = -2;
    }
}

/**
 * @brief 寻找所有的轮廓并进行一次筛选优化运行效率
 */
void DetectorB::SelectContours() {
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    imshow("threshold", binary);

    for (auto &contour: contours) {
        if (contourArea(contour) >= 3000) {
            rects.push_back(minAreaRect(contour));
            interested_contours.push_back(contour);
        }
    }
}

/**
 * @brief 从第一次找到的轮廓中筛选符合条件的角上的矩形
 * @brief 并画出符合条件的矩形
 */

bool DetectorB::SelectCornerRect() {
    for (int i = 0; i < interested_contours.size(); i++) {
        if (rects[i].size.area() < 12000) {
            if (rects[i].size.height / rects[i].size.width >= 0.75 &&
                rects[i].size.height / rects[i].size.width <= 1.33) {
                Point2f points1[4];
                rects[i].points(points1);
                for (int j = 0; j < 4; j++) {
                    line(img, points1[j], points1[(j + 1) % 4], Scalar(0, 0, 255), 5, LINE_8);
                }
                cornerRect.push_back(rects[i]);
                cornerContours.push_back(interested_contours[i]);
            }
        }
    }
    if (cornerContours.size() > 4) {
        vector<vector<Point>> selectMatchContours = cornerContours;
        cornerContours.clear();
        vector<RotatedRect> interested_rects;
        for (auto &i: selectMatchContours) {
            interested_rects.push_back(minAreaRect(i));
        }
        for (int i = 0; i < interested_rects.size(); i++) {
            if (abs(interested_rects[i].angle) <= 5) {
                cornerContours.push_back(selectMatchContours[i]);
            }
        }
        return true;
    } else if (cornerContours.size() < 4) {            //如果说识别到的矩形小于4个，则不发送任何信息
        cout << "Not enough rects" << endl;
        return false;
    }
    return false;
}



/**
 * @brief 找到被填充满的矩形并且画出其中心点
 */

void DetectorB::findFullFilledRect() {
    for (int i = 0; i < 4; i++) {
        cornerRect[i].points(point2F[i]);
        Point center(0, 0);
        for (int n = 0; n < 4; n++) {
            center.x += point2F[i][n].x;
            center.y += point2F[i][n].y;
        }
        center.x /= 4;
        center.y /= 4;
        CentersOfRects.push_back(center);
        int check = 0;
        double area = contourArea(cornerContours[i]);
        if (cornerRect[i].size.area() / area >= 0.6666 && cornerRect[i].size.area() / area <= 1.5 ) {
            check++;
            num = i;
        }
        if (check != 0) {
            FullFiilledRects.push_back(cornerRect[i]);
            number.push_back(i);
        }
    }

    for (auto &i: number) {
        circle(img, CentersOfRects[i], 2, Scalar(0, 255, 0), 2, LINE_8);
    }
}


/**
 * @brief 根据找到的矩形中心解算矿石的中心
 */

void DetectorB::SolveCenter() {
    for (int i = 0; i < 4; i++) {
        CenterOfCenter.x += CentersOfRects[i].x;
        CenterOfCenter.y += CentersOfRects[i].y;
    }
    CenterOfCenter.x /= 4;
    CenterOfCenter.y /= 4;
}


/**
 * @brief 判断此面是否有条形码
 * @return 有则为true否则为false
 */

bool DetectorB::findBarCode() {
    Mat kernel = getStructuringElement(MORPH_DILATE, Size(3, 3));
    dilate(binary, binary, kernel, Point(-1, -1), 5);
    findContours(binary, contours_barcode, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(binary, contours_barcode, -1, Scalar(0, 255, 0), 1, LINE_8);
    bool check = false;
    for (auto &i: contours_barcode) {
        if (contourArea(i) >= 10000 && contourArea(i) <= 80000) {
            RotatedRect centerRect;
            centerRect = minAreaRect(i);
            if (centerRect.size.area() / contourArea(i) <= 1.2 && centerRect.size.area() / contourArea(i) >= 1) {
                if ((centerRect.size.height / centerRect.size.width >= 0.5 &&
                     centerRect.size.height / centerRect.size.width <= 0.7) ||
                    (centerRect.size.height / centerRect.size.width >= 1.3 &&
                     centerRect.size.height / centerRect.size.width <= 1.7)) {
                    if (abs(centerRect.center.x - CenterOfCenter.x) <= 50 &&
                        abs(centerRect.center.y - CenterOfCenter.y) <= 50) {
                        check = true;
                        Point2f point[4];
                        centerRect.points(point);
                        for (int j = 0; j < 4; j++) {
                            line(img, point[j], point[(j + 1) % 4], Scalar(0, 255, 0), 5, LINE_8);
                        }
                        break;
                    }
                }
            }
        }
    }
    return check;
}

int DetectorB::JudgeSides() {
    if (FullFiilledRects.size() == 2) {
        if (findBarCode()) {
            return NoMove;
        } else {
            return Down_2;
        }
    } else {
        if (CentersOfRects[num].x < CenterOfCenter.x) {
            if (CentersOfRects[num].y < CenterOfCenter.y) {
                return Right_1;
            } else {
                return Up_1;
            }
        } else {
            if (CentersOfRects[num].y < CenterOfCenter.y) {
                return Down_1;
            } else {
                return Left_1;
            }
        }
    }
}

void DetectorB::SetData() const {
    if (Direction == 0) {               //向上90
        set = 1;
        toggle_flag = 1;
        direction = 1;
    } else if (Direction == 1) {        //向左90
        set = 0;
        toggle_flag = 1;
        direction = 1;
    } else if (Direction == 2) {        //向右90
        set = 0;
        toggle_flag = 1;
        direction = 0;
    } else if (Direction == 3) {        //向下90
        set = 1;
        toggle_flag = 1;
        direction = 0;
    } else if (Direction == 4) {        //向下180
        set = 1;
        toggle_flag = 1;
        direction = 2;
    } else if (Direction == 5) {        //不动
        toggle_flag = 0;
    }
    cout << "======" << set << direction << toggle_flag << endl;
}
