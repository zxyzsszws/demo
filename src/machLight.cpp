#include "machLight.h"
//配对
bool machLight::mach(RotatedRect& R_rect1, RotatedRect& R_rect2) {
	double ave_height = (R_rect1.size.height + R_rect2.size.height) / 2.0;
	double distance = norm(R_rect1.center - R_rect2.center);
	if (R_rect1.angle > 24 || R_rect2.angle < -24) {
		return false;
	}
	if (abs(R_rect1.angle - R_rect2.angle) > 10) {
		return false;
	}
	if (abs(R_rect1.center.y - R_rect2.center.y) > ave_height) {
		return false;
	}
	if (R_rect1.size.height / R_rect1.size.width > 7 || R_rect1.size.height / R_rect1.size.width < 1.75) {
		return false;
	}
	if (distance / ave_height < 1.5 || distance / ave_height > 4.5) {
		return false;
	}
	if (abs(R_rect1.size.height - R_rect2.size.height) > ave_height / 2.0) {
	    return false;
	}
	if (abs(R_rect1.size.height / R_rect1.size.width - R_rect1.size.height / R_rect1.size.width) > 1) {
		return false;
	}
	return true;
}

//识别
void machLight::identify(Mat& image, int flags) {
	Mat src = image.clone();
	Scalar upper;
	Scalar lower;

	//判断对什么颜色的灯条进行识别
	if (flags == 0) {
		lower = Scalar(20, 50, 100);
		upper = Scalar(40, 255, 255);
	}
	else if (flags == 1) {
		lower = Scalar(0, 30, 50);
		upper = Scalar(20, 255, 255);
	}
	else if (flags == 2) {
		lower = Scalar(100, 43, 90);
		upper = Scalar(140, 255, 255);
	}

	//先高斯双边函数让颜色差别更大
	GaussianBlur(src, src, Size(0, 0), 3);

	//转HSV便于inRanage
	cvtColor(src, src, COLOR_BGR2HSV);
	inRange(src, lower, upper, src);

	//去噪点
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(src, src, MORPH_OPEN, kernel);
	morphologyEx(src, src, MORPH_CLOSE, kernel);

	//临时的矩形
	vector<vector<Point>> P_contours;
	vector<RotatedRect> temp_rects;
	findContours(src, P_contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat dst = Mat::zeros(src.size(), CV_8SC3);
	dst = Scalar(255, 255, 255);
	for (int i = 0; i < P_contours.size(); i++) {
		RotatedRect temp_rect = minAreaRect(P_contours[i]);
		ellipse(dst, temp_rect, Scalar(0, 0, 255), 2, LINE_AA);
		//将角度转化为-45° - 45°之间
		if (temp_rect.angle > 45 && temp_rect.size.width > temp_rect.size.height) {
			temp_rect.angle -= 90;
			double temp = temp_rect.size.width;
			temp_rect.size.width = temp_rect.size.height;
			temp_rect.size.height = temp;
		}
		temp_rects.push_back(temp_rect);
	}

	//将匹配的灯条分别放入list_Lists;
	vector<int> index;//记录已匹配灯条的下表
	vector<RotatedRect> temp_lists;
	for (int i = 0; i < temp_rects.size(); i++) {
		bool isRepeat = false;
		for (auto it : index) {
			if (it == i) {
				isRepeat = true;
				break;
			}
		}
		for (int j = i + 1; j < temp_rects.size() && !isRepeat; j++) {
			if (this->mach(temp_rects[i], temp_rects[j]) && !isRepeat) {
				temp_lists.push_back(temp_rects[i]);
				temp_lists.push_back(temp_rects[j]);
				this->list_lights.push_back(temp_lists);
				temp_lists.clear();
				index.push_back(j);
				break;
			}
		}
	}
}
