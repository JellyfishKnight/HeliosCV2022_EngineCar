#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Detector.h"
#include "SerialPort.h"

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    VideoCapture capture(2);             //此处的index自定
    if (!capture.isOpened()) {
        cout << "Wrong Index or Not Connected! " << endl;
        return 1;
    }
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);
    capture.set(CAP_PROP_FRAME_WIDTH, 640);
    Serial serial;
    serial.InitPort();
    while (true) {
        capture >> frame;
        if (frame.empty()) {
            cout << "Image is empty!" << endl;
            break;
        }
        flip(frame, frame, 1);
        DetectorB detector(frame);
        detector.detect();
        frame = detector.getImg();
        imshow("frame", frame);
        detector.SetData();
        serial.pack(set, direction, toggle_flag);
        serial.WriteData();
        int c = waitKey(1);
        if (c == 27) {
            break;
        }
    }
    return 0;
}