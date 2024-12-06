#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <cmath>
using namespace std;
using namespace cv;
class getNumber {
public:
	vector<vector<Mat>> num_dst;
	
	getNumber();//获取文件夹里的数字图片并存在num_dst中
	
	int Mach_number(Mat& image);//返回image对应的数字
	
	int sumMat(Mat& image);//计算图片所有像素点和
	
	Mat interceptMat(vector<RotatedRect> list_rects, Mat& image, int flags);//放回灯条范围内的二值图片
};
