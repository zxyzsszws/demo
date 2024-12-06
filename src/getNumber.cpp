#include "getNumber.h"

getNumber::getNumber() {
	vector<Mat> temp;
	for (int i = 0; i < 4; i++) {
	    for (int j = 0; j < 8; j++) {
	        char num1 = i + '1';
	        char num2 = j + '0';
		    string file = "/home/zhouxingyuan/下载/opencv-4.7.0/Dmo/lib/";
		    file += num1;
		    file += num2;
		    file += ".png";
		    Mat dst = imread(file, IMREAD_GRAYSCALE);
		    resize(dst, dst, Size(100, 100), 0, 0);
		    temp.push_back(dst);
	    }
	    this->num_dst.push_back(temp);
	    temp.clear();
	}
	Mat dst = imread("/home/zhouxingyuan/下载/opencv-4.7.0/Dmo/lib/70.png", IMREAD_GRAYSCALE);
	resize(dst, dst, Size(100, 100), 0, 0);
	temp.push_back(dst);
	this->num_dst.push_back(temp);
}

int getNumber::sumMat(Mat& image) {
	int sum = 0;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			sum += static_cast<int>(image.at<uchar>(i, j));
		}
	}
	return sum;
}

Mat getNumber::interceptMat(vector<RotatedRect> list_rects, Mat& image, int flags) {
	//截取灯条范围内的图片区域
	double X1 = list_rects[0].center.x;
    double Y1 = list_rects[0].center.y;
    double X2 = list_rects[1].center.x;
    double Y2 = list_rects[1].center.y;
    double center_x = (X1 + X2) / 2.0;
    double center_y = (Y1 + Y2) / 2.0;
    double level_distance = abs(X1 - X2);
    double x1 = (center_x - level_distance / 2.0) > 0 ? center_x - level_distance / 2.0 : 0;
    double x2 = center_x + level_distance / 2.0 < image.cols ? center_x + level_distance / 2.0 : image.cols;
    double y1 = (center_y - level_distance / 2.0) > 0 ? center_y - level_distance / 2.0 : 0;
    double y2 = (center_y + level_distance / 2.0) < image.rows ? center_y + level_distance / 2.0 : image.rows;
    Rect rect(Point(x1, y1), Point(x2, y2));
    Mat src = image(rect);

	//二值化
	resize(src, src, Size(100, 100), 0, 0);
	cvtColor(src, src, COLOR_BGR2GRAY);
	int thresh;
	if (flags == 0)
		thresh = 25;
	else if (flags == 1)
		thresh = 140;
	else
		thresh = 20;
	threshold(src, src, thresh, 255, THRESH_BINARY_INV);

	//降噪
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(src, src, MORPH_OPEN, kernel);
	morphologyEx(src, src, MORPH_CLOSE, kernel);
	return src;
}

int getNumber::Mach_number(Mat& image) {
	for (int i = 0; i < this->num_dst.size() - 1; i++) {
	    for (int j = 0; j < 8; j++) {
	        Mat diff;
		    absdiff(image, this->num_dst[i][j], diff);
		    if (this->sumMat(diff) < 320000) {
			    return i + 1;
		    }
	    }
	}
	Mat diff;
	absdiff(this->num_dst[4][0], image, diff);
	if (this->sumMat(diff) < 320000) {
	    return 4;
	}
	return -1;
}

