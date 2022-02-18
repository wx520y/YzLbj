#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include"opencv.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/operations.hpp"
using namespace cv;
//struct lackInfo
//{
//	Rect rect_loc;
//	int num;
//};
struct defectRegion
{
	Rect rect_loc;
	int type; //type = 0,表示未断裂，type = 1表示断裂
};
struct lackInfo
{
	std::vector<defectRegion> rectInfo;
	int nDefectNum; //缺陷篦条个数，包括断裂和磨损
	float ratio; //磨损和缺陷篦条的面积占图像整个面积的比例
};

// 内轮廓填充   
// 参数:   
// 1. pBinary: 输入二值图像，单通道，位深IPL_DEPTH_8U。  
// 2. dAreaThre: 面积阈值，当内轮廓面积小于等于dAreaThre时，进行填充。  
void FillInternalContours(IplImage *pBinary, double dAreaThre);

//横向投影，找空行
void getEmptyRow(Mat src, std::vector<int>&rowValue);

//旋转图像
Mat rotateImage(Mat matSrc, int degree, Point center1, Point &rotate_center);

float getPixNumRelation(Mat img, Rect rect);
//int detectQueshi_test(Mat src,int index, Rect &rect_loc, int &num);
/*
功  能：检测篦板是否存在缺失
输  入：参数1，图像数据（输入）
       参数2，篦条缺失位置（输出）
	   参数3，缺失的篦条个数（输出）
返回值: 1，有缺失
       0，无缺失
*/
int getSobelImg(Mat src, Mat &sobelImg);
int detectMosun(Mat src, lackInfo &info);
//int detectQueshi(Mat src, Rect &rect_loc, int &num);
//int detectQueshi(Mat src, std::vector<lackInfo> &info);
/*
功能：检测壁板是否归位
输入:图像数据（输入）
返回值：0，表示未归位
       1，表示归位
*/
int detectGuiwei(Mat src);
