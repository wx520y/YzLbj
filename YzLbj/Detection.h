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
	int type; //type = 0,��ʾδ���ѣ�type = 1��ʾ����
};
struct lackInfo
{
	std::vector<defectRegion> rectInfo;
	int nDefectNum; //ȱ�������������������Ѻ�ĥ��
	float ratio; //ĥ���ȱ�����������ռͼ����������ı���
};

// ���������   
// ����:   
// 1. pBinary: �����ֵͼ�񣬵�ͨ����λ��IPL_DEPTH_8U��  
// 2. dAreaThre: �����ֵ�������������С�ڵ���dAreaThreʱ��������䡣  
void FillInternalContours(IplImage *pBinary, double dAreaThre);

//����ͶӰ���ҿ���
void getEmptyRow(Mat src, std::vector<int>&rowValue);

//��תͼ��
Mat rotateImage(Mat matSrc, int degree, Point center1, Point &rotate_center);

float getPixNumRelation(Mat img, Rect rect);
//int detectQueshi_test(Mat src,int index, Rect &rect_loc, int &num);
/*
��  �ܣ���������Ƿ����ȱʧ
��  �룺����1��ͼ�����ݣ����룩
       ����2������ȱʧλ�ã������
	   ����3��ȱʧ�����������������
����ֵ: 1����ȱʧ
       0����ȱʧ
*/
int getSobelImg(Mat src, Mat &sobelImg);
int detectMosun(Mat src, lackInfo &info);
//int detectQueshi(Mat src, Rect &rect_loc, int &num);
//int detectQueshi(Mat src, std::vector<lackInfo> &info);
/*
���ܣ����ڰ��Ƿ��λ
����:ͼ�����ݣ����룩
����ֵ��0����ʾδ��λ
       1����ʾ��λ
*/
int detectGuiwei(Mat src);
