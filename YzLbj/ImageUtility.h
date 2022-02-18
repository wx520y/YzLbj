#pragma once

#include "stdafx.h"
#include "atlimage.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class ImageUtility
{
public:
	ImageUtility(void);
	~ImageUtility(void);
	// 实现cv::Mat 结构到 CImage结构的转化
	static void MatToCImage(Mat& mat, CImage& cImage);
	//根据控件大小重新定义图像的尺寸
	static void ResizeMat(cv::Mat &Image, int width, int height);
	//将图片在控件上显示
	static void DisplayImage(cv::Mat img, UINT ID,HWND hDlg);
};
