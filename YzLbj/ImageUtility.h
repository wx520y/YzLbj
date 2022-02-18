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
	// ʵ��cv::Mat �ṹ�� CImage�ṹ��ת��
	static void MatToCImage(Mat& mat, CImage& cImage);
	//���ݿؼ���С���¶���ͼ��ĳߴ�
	static void ResizeMat(cv::Mat &Image, int width, int height);
	//��ͼƬ�ڿؼ�����ʾ
	static void DisplayImage(cv::Mat img, UINT ID,HWND hDlg);
};
