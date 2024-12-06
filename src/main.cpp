#include<opencv2/opencv.hpp>
#include<iostream>
#include "machLight.h"
#include "getNumber.h"
using namespace std;
using namespace cv;
int main() {
    machLight machlight;
    getNumber getnum;
    string str = "/home/zhouxingyuan/下载/opencv-4.7.0/Dmo/lib/test";
    //index:
    //1.test1
    //2.test2
    //3.test3
    char index = '3';
    str += index;
    if (index == '1') {
        str += ".avi";
    }
    else {
        str += ".mp4";
    }
    cout << str << endl;
    VideoCapture capture(str);
    Mat image;
    while (1) {
	capture.read(image);
	int c = waitKey(1);
	if (image.empty()) {
		break;
	}
	if (c == 27) {
		break;
	}
	int flags;
	Scalar color;
	if (index == '1') {
		flags = 0;
  		color = Scalar(255, 0, 0);
	}
	else if (index == '2') {
		flags = 1;
  	    color = Scalar(255, 0, 0);
	}
	else {
		flags = 2;
		color = Scalar(0, 0, 255);
	}
	machlight.identify(image, flags);
	for (int i = 0; i < machlight.list_lights.size(); i++) {
	        Mat dst = getnum.interceptMat(machlight.list_lights[i], image, flags);
	        string mark = "";
	        if (getnum.Mach_number(dst) != -1) {
		        mark += getnum.Mach_number(dst) + '0';
		        double x1 = machlight.list_lights[i][0].center.x;
		        double x2 = machlight.list_lights[i][1].center.x;
		        double y1 = machlight.list_lights[i][0].center.y;
		        double y2 = machlight.list_lights[i][1].center.y;
		        putText(image, mark, Point((x1 + x2) / 2.0, (y1 + y2) / 2.0), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2, LINE_8);
	        }
	        ellipse(image, machlight.list_lights[i][0], color, 3, LINE_8);
	        ellipse(image, machlight.list_lights[i][1], color, 3, LINE_8);
        }
	imshow("window1", image);
	machlight.list_lights.clear();
    }
    waitKey(0);
    destroyAllWindows();
    return 0;
}
