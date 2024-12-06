#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>
using namespace std;
using namespace cv;

class machLight {
public:
	vector<vector<RotatedRect>> list_lights;//存储匹配的灯条

	bool mach(RotatedRect& R_rect1, RotatedRect& R_rect2);//查看两个矩形是否匹配;

	void identify(Mat& image, int flags);//识别图片并存储矩形flags:0.test1（yellow）；1.test2（red）；2.test3（blue）；
};
