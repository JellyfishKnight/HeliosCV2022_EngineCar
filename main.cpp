#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Detector/Detector.h"

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    VideoCapture capture(0);             //此处的index自定
    if (!capture.isOpened()) {
        cout << "Wrong Index or Not Connected! " << endl;
        return 1;
    }
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);
    capture.set(CAP_PROP_FRAME_WIDTH, 640);
    //Serial serial;
    //serial.InitPort();
    while (true) {
        capture >> frame;
        if (frame.empty()) {
            cout << "Image is empty!" << endl;
            break;
        }
        flip(frame, frame, 1);
        DetectorB detectorB(frame);
        detectorB.detect();
        frame = detectorB.getImg();
        imshow("frame", frame);
        //serial.pack();
        //serial.WriteData();
        int c = waitKey(1);
        if (c == 27) {
            break;
        }
    }
    return 0;
}