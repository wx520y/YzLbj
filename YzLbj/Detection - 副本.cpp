#pragma once
#include "stdafx.h"
#include "Detection.h"
#include <iostream>
#include <vector>

#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

// yangzhouLianbijiDetect.cpp : 定义控制台应用程序的入口点。
//

//#define showResult
//#define printResult
// 内轮廓填充   
// 参数:   
// 1. pBinary: 输入二值图像，单通道，位深IPL_DEPTH_8U。  
// 2. dAreaThre: 面积阈值，当内轮廓面积小于等于dAreaThre时，进行填充。   
void FillInternalContours(IplImage *pBinary, double dAreaThre)
{
	double dConArea;
	CvSeq *pContour = NULL;
	CvSeq *pConInner = NULL;
	CvMemStorage *pStorage = NULL;
	// 执行条件   
	if (pBinary)
	{
		// 查找所有轮廓   
		pStorage = cvCreateMemStorage(0);
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		// 填充所有轮廓   
		cvDrawContours(pBinary, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));
		// 外轮廓循环   
		int wai = 0;
		int nei = 0;
		for (; pContour != NULL; pContour = pContour->h_next)
		{
			wai++;
			// 内轮廓循环   
			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)
			{
				nei++;
				// 内轮廓面积   
				dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ));
				//printf("%f\n", dConArea);
				if (dConArea <= dAreaThre)
				{
					cvDrawContours(pBinary, pConInner, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));
				}
			}
		}
		//printf("wai = %d, nei = %d", wai, nei);
		cvReleaseMemStorage(&pStorage);
		pStorage = NULL;
	}
}
//横向投影，找空行
void getEmptyRow(Mat src, vector<int>&rowValue)
{
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < height; i++)
	{
		int value = 0;
		for (int j = 0; j < width; j++)
		{
			if (int(src.at<uchar>(i, j)) == 255)
			{
				value += 1;
			}
		}
		float ratio = 0;
		ratio = value * 1.0 / width;
		//cout << ratio << ",";
		if (ratio > 0.85)
		{
			//cout << "i = " << i << ",value = " << value << endl;
			rowValue.push_back(i);
		}
	}
}
//旋转图像
Mat rotateImage(Mat matSrc, int degree, Point center1, Point &rotate_center)
{
	//degree = -degree;
	double angle = degree * CV_PI / 180.;
	double alpha = cos(angle);
	double beta = sin(angle);
	int iWidth = matSrc.cols;
	int iHeight = matSrc.rows;
	int iNewWidth = cvRound(iWidth * fabs(alpha) + iHeight * fabs(beta));
	int iNewHeight = cvRound(iHeight * fabs(alpha) + iWidth * fabs(beta));

	double m[6];
	m[0] = alpha;
	m[1] = beta;
	m[2] = (1 - alpha) * iWidth / 2. - beta * iHeight / 2.;
	m[3] = -m[1];
	m[4] = m[0];
	m[5] = beta * iWidth / 2. + (1 - alpha) * iHeight / 2.;
	m[2] += (iNewWidth - iWidth) / 2;
	m[5] += (iNewHeight - iHeight) / 2;
	cv::Mat M = cv::Mat(2, 3, CV_64F, m);
	cv::Mat matDst1 = cv::Mat(cv::Size(iNewWidth, iNewHeight), matSrc.type(), cv::Scalar::all(0));

	double D = m[0] * m[4] - m[1] * m[3];
	D = D != 0 ? 1. / D : 0;
	double A11 = m[4] * D, A22 = m[0] * D;
	m[0] = A11; m[1] *= -D;
	m[3] *= -D; m[4] = A22;
	double b1 = -m[0] * m[2] - m[1] * m[5];
	double b2 = -m[3] * m[2] - m[4] * m[5];
	m[2] = b1; m[5] = b2;

	//int round_delta = 512;//nearest
	for (int y = 0; y<iNewHeight; ++y)
	{
		for (int x = 0; x<iNewWidth; ++x)
		{

			int tmpx = cvFloor(m[0] * x + m[1] * y + m[2]);
			int tmpy = cvFloor(m[3] * x + m[4] * y + m[5]);
			/*int adelta = cv::saturate_cast<int>(m[0] * x * 1024);
			int bdelta = cv::saturate_cast<int>(m[3] * x * 1024);
			int X0 = cv::saturate_cast<int>((m[1] * y + m[2]) * 1024) + round_delta;
			int Y0 = cv::saturate_cast<int>((m[4] * y + m[5]) * 1024) + round_delta;
			int X = (X0 + adelta) >> 10;
			int Y = (Y0 + bdelta) >> 10;*/

			/*if ((unsigned)X < iWidth && (unsigned)Y < iHeight)
			{
			matDst1.at<uchar>(y, x) = matSrc.at<uchar>(Y, X);
			}*/
			//if ((unsigned)tmpx < (unsigned)iWidth && (unsigned)tmpy < (unsigned)iHeight)
			if (tmpx > 0 && tmpy > 0 && tmpx < iWidth && tmpy < iHeight)
			{
				if (tmpy == center1.y && tmpx == center1.x)
				{
					//cout << tmpy << "," << tmpx << endl;
					rotate_center.x = x;
					rotate_center.y = y;
				}
				matDst1.at<uchar>(y, x) = matSrc.at<uchar>(tmpy, tmpx);
			}
		}
	}
	//imshow("dst", matDst1);
	return matDst1;
}
/*
输入：二值化图，ROI区域
输出：0，正常
1，异常，有缺失
*/
int compare1(const void *a, const void *b)
{
	int *p1 = (int *)a;
	int *p2 = (int *)b;
	return (*p2 - *p1);
}

struct XLineInfo
{
	int xIndex;
	int symbol;
};
struct missLocInfo
{
	Rect rect;
	int useful;
};
void removeBall(Mat &img_temp)
{
	int height = img_temp.rows;
	int width = img_temp.cols;
	//补洞
	threshold(img_temp, img_temp, 128, 255, CV_THRESH_BINARY_INV);
	IplImage img_temp2 = img_temp;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i <= 5 || i >= height - 5 || j <= 5 || j >= width - 5)
			{
				img_temp.at<uchar>(i, j) = 255;
			}
		}
	}
	FillInternalContours(&img_temp2, 400);
	threshold(img_temp, img_temp, 128, 255, CV_THRESH_BINARY_INV);
}
void getBall(Mat beforeImg, Mat afterImg, Mat &ballImg)
{
	int height = beforeImg.rows;
	int width = beforeImg.cols;
	//制作掩模，主要目的是去除边缘轮廓
	Mat mask = Mat::zeros(beforeImg.size(), CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i >= 5 && i <= height - 5 && j >= 5 && j <= width - 5)
			{
				mask.at<uchar>(i, j) = 255;
			}
		}
	}
	//imshow("mask", mask);
	bitwise_xor(afterImg, beforeImg, ballImg, mask);
}
void getBallLoc(Mat imgHole, vector<Rect> &holeList)
{
	//找轮廓，确定有没有小球
	Mat dstImg_RGB = Mat::zeros(imgHole.size(), CV_8UC3);
	cvtColor(imgHole, dstImg_RGB, CV_GRAY2BGR);
	Mat imgHole_temp = imgHole.clone();

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(imgHole_temp, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	vector<RotatedRect> box_gun(contours.size());
	int contourNum = 0;

	if (contours.size() > 0)
	{
		for (size_t i = 0; i < contours.size(); i++)
		{
			int area = 0;
			area = contourArea(contours[i]);
			//cout << i << ":=" << area << endl;
			if (contours[i].size() > 5 && area >= 60 && area < 350)//小球的面积阈值的取值是这个要通过多次测试
			{
				//cout << "area = " << area << endl;
				Rect boundRect;
				boundRect = boundingRect(contours[i]);
				//cout << "ball-width = " << boundRect.width << endl;
				//cout << "ball-height = " << boundRect.height << endl;
				int bigValue = max(boundRect.width, boundRect.height);
				int smallValue = min(boundRect.width, boundRect.height);
				float ratio = 0;
				ratio = bigValue * 1.0 / smallValue;
				//cout << "ratio = " << ratio << endl;
				if (bigValue < 45 && smallValue >= 7 && ratio <= 5)//bigValue的值有可能是两颗小球，所以设的比较大，相应的ratio的值也较大
				{
					rectangle(dstImg_RGB, boundRect, Scalar(255, 255, 0), 1, CV_AA);
					holeList.push_back(boundRect);
				}
			}
		}
	}

}
float getResult_before(Mat img, Rect rect)
{
	int ret = 0;
	int height = rect.height;
	int width = rect.width;
	Mat img_init = Mat(img, rect).clone(); //二值化后的图
	Mat img_temp = Mat(img, rect).clone(); //
	Mat imgROI_before = img_init.clone(); //去小球之前的二值化图
										  //int isHasHole = 0;
	int isMiss = 0;
	removeBall(img_temp);
	Mat imgROI_after = Mat(img_temp); //去小球之后的二值化图

	Mat imgHole = Mat::zeros(cvSize(width, height), CV_8UC1); //纯粹小球的图
	vector<Rect>holeList; //检测到的所有小球的外接矩形
	getBall(imgROI_before, imgROI_after, imgHole);
	getBallLoc(imgHole, holeList); //获取小球的位置信息

								   //imshow("hole", imgHole);

								   //判断是否存在缺失
	Mat imgROI_after1 = Mat::zeros(imgROI_before.size(), CV_8UC1);
	//dilate(imgROI_after, imgROI_after, element, cvPoint(-1, -1), 1);
	Mat mask = Mat::zeros(imgROI_after1.size(), CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i >= 5 && i <= height - 5 && j >= 5 && j <= width - 5)
			{
				mask.at<uchar>(i, j) = 255;
			}
		}
	}
	bitwise_and(imgROI_after, mask, imgROI_after1, mask);
	//imshow("imgROI_after", imgROI_after1);

	//纵向投影
	vector<int>colWhiteNum;
	for (int j = 5; j < imgROI_after1.cols - 5; j++) //因为Mask的边界是5
	{
		int whitekNum = 0;
		int i = 0;
		vector<int> rowValue;
		for (i = 5; i < imgROI_after1.rows - 5; i++)
		{
			if (imgROI_after1.at<uchar>(i, j) == 255)
			{
				whitekNum++;
			}
		}
		colWhiteNum.push_back(whitekNum);
		//cout << "j = " << j << "，value = " << whitekNum << endl;
	}
	//cout << "结束纵向投影" << endl;
	//确定纵向间隔的坐标

	vector<XLineInfo> zeroIndex;
	for (int i = 0; i < colWhiteNum.size() - 1; i++)
	{
		XLineInfo info;
		if (colWhiteNum[i] == 0 && colWhiteNum[i + 1] > 0)
		{
			info.xIndex = i + 5;
			info.symbol = 1;
			zeroIndex.push_back(info);
		}
		if (colWhiteNum[i] > 0 && colWhiteNum[i + 1] == 0)
		{
			info.xIndex = i + 5;
			info.symbol = 2;
			zeroIndex.push_back(info);
		}

	}

	Mat dstImg_RGB1 = Mat::zeros(imgROI_after1.size(), CV_8UC3);
	cvtColor(imgROI_after1, dstImg_RGB1, CV_GRAY2BGR);
	vector<int> XzeroIndex;
	vector<missLocInfo> missLineIndex;
	for (int i = 0; i < zeroIndex.size(); i++)
	{
		if (i == 0 && zeroIndex[i].symbol == 2) //忽略首次出现2的，不可能出现连续222的情况，所以只考虑下标为0的点即可。
		{
			continue;
		}
		if (zeroIndex[i].symbol == 1)
		{
			int begin = 0;
			int end = 0;
			if (i + 1 >= zeroIndex.size())//说明最后一位是1，此时补最后面一列
			{
				begin = zeroIndex[i].xIndex;
				end = width - 5;
				if (end - begin < 20) //特别边缘的，认为是无效的
				{
					continue;
				}
			}
			else
			{
				begin = zeroIndex[i].xIndex;
				end = zeroIndex[i + 1].xIndex;
			}
			vector<int> lineNumList;
			for (int i1 = 5; i1 < height - 5; i1++)
			{
				int sumwhiteNum = 0;
				for (int j1 = begin; j1 < end; j1++)
				{
					if (imgROI_after1.at<uchar>(i1, j1) == 255)
					{
						sumwhiteNum += 1;
					}
				}
				//cout << sumwhiteNum << endl;
				lineNumList.push_back(sumwhiteNum);
			}
			//确定缺失点行号
			int symbol = 0;
			int beginY = 0;
			int endY = 0;
			for (int i1 = 0; i1 < lineNumList.size(); i1++)
			{
				if (lineNumList[i1] == 0)
				{
					if (symbol == 0)
					{
						symbol = 1;
						beginY = i1 + 5;
					}
					else
					{
						if (i1 == lineNumList.size() - 1)
						{
							endY = i1 + 5;
							if (endY - beginY >= 0.6 * height || (beginY < 10 && endY - beginY < 10) || endY - beginY < 4)
							{
								symbol = 0;
								continue;
							}

							Rect rect;
							rect.x = begin;
							rect.y = beginY;
							rect.width = end - begin;
							rect.height = endY - beginY;
							missLocInfo temp;
							temp.rect = rect;
							temp.useful = 1;
							missLineIndex.push_back(temp);


						}
					}
				}
				else
				{
					if (symbol == 1)
					{
						endY = i1 + 5;
						if (endY - beginY >= 0.6 * height || (beginY < 10 && endY - beginY < 10) || endY - beginY < 4)
						{
							symbol = 0;
							continue;
						}
						Rect rect;
						rect.x = begin;
						rect.y = beginY;
						rect.width = end - begin;
						rect.height = endY - beginY;
						missLocInfo temp;
						temp.rect = rect;
						temp.useful = 1;
						missLineIndex.push_back(temp);

					}
					symbol = 0;
				}
			}

		}
	}
	//根据找到的缺失位置坐标，找横向附近有没有小球holeList即可
	if (missLineIndex.size() == 0)
	{
		ret = 0; //没有缺失
	}
	else
	{
		for (int i = 0; i < missLineIndex.size(); i++)
		{
			int midMissY = missLineIndex[i].rect.y + missLineIndex[i].rect.height / 2;
			int midMissX = missLineIndex[i].rect.x + missLineIndex[i].rect.width / 2;

			for (int j = 0; j < holeList.size(); j++) //这是和检测到的小球的位置进行比较，
			{
				int midHoleX = holeList[j].x + holeList[j].width / 2;
				if (midMissY >= holeList[j].y && midMissY <= holeList[j].y + holeList[j].height)
				{
					//cout << abs(midMissX - midHoleX) << endl;
					if (abs(midMissX - midHoleX)> 50)
					{
						missLineIndex[i].useful = 1; //旁边没有小球
					}
					else
					{
						missLineIndex[i].useful = 0; //旁边有小球
					}
				}

			}
			if (missLineIndex[i].useful == 1)
			{

				//img_temp,和这个图上的周围区域比较//小球没分割出来的情况
				int beginCol = missLineIndex[i].rect.x;
				int endCol = beginCol + missLineIndex[i].rect.width;
				int length = 10; //经验值
				int symbol = 0;
				for (int col = max(0, beginCol - length); col < min(endCol + length, width); col++)
				{
					if (img_init.at<uchar>(midMissY, col) == 255)
					{
						missLineIndex[i].useful = 0;
						symbol = 1;
						break;
					}
				}
				if (symbol == 0)
				{
					missLineIndex[i].useful = 1;
				}
			}
		}
	}
	int missNum = 0;
	for (int i = 0; i < missLineIndex.size(); i++)
	{
		if (missLineIndex[i].useful == 1)
		{
			rectangle(dstImg_RGB1, missLineIndex[i].rect, Scalar(0, 0, 255), 1, CV_AA);
			missNum++;
		}
	}

	if (missNum > 0)
	{
		cout << "一共有" << missNum << "个真正缺失" << endl;
		ret = 1;
	}
	//最终根据不同情况确定是否是真正的缺失
	return ret;
}
//判断篦条是否缺失，包括真正缺失和阴影造成的伪缺失
//返回值0表示没有缺失，返回值>=1表示有缺失
int isMiss(Mat imgROI_after, vector<missLocInfo> &missLineIndex)
{
	int ret = 0;
	int height = imgROI_after.rows;
	int width = imgROI_after.cols;
	Mat imgROI_after1 = Mat::zeros(imgROI_after.size(), CV_8UC1);
	//dilate(imgROI_after, imgROI_after, element, cvPoint(-1, -1), 1);
	Mat mask = Mat::zeros(imgROI_after1.size(), CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i >= 5 && i <= height - 5 && j >= 5 && j <= width - 5)
			{
				mask.at<uchar>(i, j) = 255;
			}
		}
	}
	bitwise_and(imgROI_after, mask, imgROI_after1, mask);

	//纵向投影
	vector<int>colWhiteNum;
	for (int j = 5; j < imgROI_after1.cols - 5; j++) //因为Mask的边界是5
	{
		int whitekNum = 0;
		int i = 0;
		vector<int> rowValue;
		for (i = 5; i < imgROI_after1.rows - 5; i++)
		{
			if (imgROI_after1.at<uchar>(i, j) == 255)
			{
				whitekNum++;
			}
		}
		colWhiteNum.push_back(whitekNum);
		//cout << "j = " << j << "，value = " << whitekNum << endl;
	}
	//cout << "结束纵向投影" << endl;
	//确定纵向间隔的坐标

	vector<XLineInfo> zeroIndex;
	for (int i = 0; i < colWhiteNum.size() - 1; i++)
	{
		XLineInfo info;
		if (colWhiteNum[i] == 0 && colWhiteNum[i + 1] > 0)
		{
			info.xIndex = i + 5;
			info.symbol = 1;
			zeroIndex.push_back(info);
		}
		if (colWhiteNum[i] > 0 && colWhiteNum[i + 1] == 0)
		{
			info.xIndex = i + 5;
			info.symbol = 2;
			zeroIndex.push_back(info);
		}

	}

	vector<int> XzeroIndex;

	for (int i = 0; i < zeroIndex.size(); i++)
	{
		if (i == 0 && zeroIndex[i].symbol == 2) //忽略首次出现2的，不可能出现连续222的情况，所以只考虑下标为0的点即可。
		{
			continue;
		}
		if (zeroIndex[i].symbol == 1)
		{
			int begin = 0;
			int end = 0;
			if (i + 1 >= zeroIndex.size())//说明最后一位是1，此时补最后面一列
			{
				begin = zeroIndex[i].xIndex;
				end = width - 5;
				if (end - begin < 20) //特别边缘的，认为是无效的
				{
					continue;
				}
			}
			else
			{
				begin = zeroIndex[i].xIndex;
				end = zeroIndex[i + 1].xIndex;
			}
			vector<int> lineNumList;
			for (int i1 = 5; i1 < height - 5; i1++)
			{
				int sumwhiteNum = 0;
				for (int j1 = begin; j1 < end; j1++)
				{
					if (imgROI_after1.at<uchar>(i1, j1) == 255)
					{
						sumwhiteNum += 1;
					}
				}
				//cout << sumwhiteNum << endl;
				lineNumList.push_back(sumwhiteNum);
			}
			//确定缺失点行号
			int symbol = 0;
			int beginY = 0;
			int endY = 0;
			for (int i1 = 0; i1 < lineNumList.size(); i1++)
			{
				if (lineNumList[i1] == 0)
				{
					if (symbol == 0)
					{
						symbol = 1;
						beginY = i1 + 5;
					}
					else
					{
						if (i1 == lineNumList.size() - 1)
						{
							endY = i1 + 5;
							if (endY - beginY >= 0.6 * height || (beginY < 10 && endY - beginY < 10) || endY - beginY < 4)
							{
								symbol = 0;
								continue;
							}

							Rect rect;
							rect.x = begin;
							rect.y = beginY;
							rect.width = end - begin;
							rect.height = endY - beginY;
							missLocInfo temp;
							temp.rect = rect;
							temp.useful = 1;
							missLineIndex.push_back(temp);


						}
					}
				}
				else
				{
					if (symbol == 1)
					{
						endY = i1 + 5;
						if (endY - beginY >= 0.6 * height || (beginY < 10 && endY - beginY < 10) || endY - beginY < 4)
						{
							symbol = 0;
							continue;
						}
						Rect rect;
						rect.x = begin;
						rect.y = beginY;
						rect.width = end - begin;
						rect.height = endY - beginY;
						missLocInfo temp;
						temp.rect = rect;
						temp.useful = 1;
						missLineIndex.push_back(temp);

					}
					symbol = 0;
				}
			}

		}
	}

	ret = missLineIndex.size();
	return ret;
}
//判断篦条是否缺失，返回值0表示没有缺失，返回值大于0表示缺失的个数
int getResult(Mat img, Rect rect)
{
	int ret = 0;
	int height = rect.height;
	int width = rect.width;
	Mat img_init = Mat(img, rect).clone(); //二值化后的图
	Mat img_temp = Mat(img, rect).clone(); //
	Mat imgROI_before = img_init.clone(); //去小球之前的二值化图
										  //int isHasHole = 0;
	removeBall(img_temp);
	Mat imgROI_after = Mat(img_temp); //去小球之后的二值化图

	Mat imgHole = Mat::zeros(cvSize(width, height), CV_8UC1); //纯粹小球的图
	vector<Rect>holeList; //检测到的所有小球的外接矩形
	getBall(imgROI_before, imgROI_after, imgHole);
	getBallLoc(imgHole, holeList); //获取小球的位置信息

								   //imshow("hole", imgHole);

								   //判断是否存在缺失
	int result = 0;
	vector<missLocInfo> missLineIndex;
	result = isMiss(imgROI_after, missLineIndex);

	//根据找到的缺失位置坐标，找横向附近有没有小球holeList即可
	if (result == 0)
	{
		ret = 0; //没有缺失
	}
	else
	{
		for (int i = 0; i < missLineIndex.size(); i++)
		{
			int midMissY = missLineIndex[i].rect.y + missLineIndex[i].rect.height / 2;
			int midMissX = missLineIndex[i].rect.x + missLineIndex[i].rect.width / 2;

			for (int j = 0; j < holeList.size(); j++) //这是和检测到的小球的位置进行比较，
			{
				int midHoleX = holeList[j].x + holeList[j].width / 2;
				if (midMissY >= holeList[j].y && midMissY <= holeList[j].y + holeList[j].height)
				{
					//cout << abs(midMissX - midHoleX) << endl;
					if (abs(midMissX - midHoleX)> 50)
					{
						missLineIndex[i].useful = 1; //旁边没有小球
					}
					else
					{
						missLineIndex[i].useful = 0; //旁边有小球
					}
				}

			}
			if (missLineIndex[i].useful == 1)
			{

				//img_temp,和这个图上的周围区域比较,这是应用于小球没分割出来的情况
				int beginCol = missLineIndex[i].rect.x;
				int endCol = beginCol + missLineIndex[i].rect.width;
				int length = 10; //经验值
				int symbol = 0;
				for (int col = max(0, beginCol - length); col < min(endCol + length, width); col++)
				{
					if (col >= min(beginCol + 3, width) && col <= max(0, endCol - 3))
					{
						continue;
					}
					if (img_init.at<uchar>(midMissY, col) == 255)
					{
						missLineIndex[i].useful = 0;
						symbol = 1;
						break;
					}
				}
				if (symbol == 0)
				{
					missLineIndex[i].useful = 1;
				}
			}
		}
	}
	int missNum = 0;
	for (int i = 0; i < missLineIndex.size(); i++)
	{
		if (missLineIndex[i].useful == 1)
		{
			missNum++;
		}
	}


	ret = missNum;
	return ret;
}
float getPixNumRelation(Mat img, Rect rect)
{
	Mat img_temp2 = img.clone();
	//imshow("temp2", img_temp2);
	//补洞
	threshold(img, img, 128, 255, CV_THRESH_BINARY_INV);
	IplImage img_temp = img;
	FillInternalContours(&img_temp, 350);
	threshold(img, img, 128, 255, CV_THRESH_BINARY_INV);

	//imshow("after",img);
	Mat img_temp1 = Mat(img, rect);
	//imshow("temp", img_temp1);
	//imwrite("test1.bmp", img_temp1);
	waitKey(10);
	int ret = 0;
	float ratio = 0;
	int blackNum = 0;
	/*for (size_t i = rect.y; i < rect.height + rect.y; i++)
	{
	for (size_t j = rect.x; j < rect.width + rect.x; j++)
	{
	if (img.at<uchar>(i,j) == 0)
	{
	blackNum++;
	}
	}
	}
	ratio = blackNum * 1.0/ max(rect.height * rect.width - blackNum,1);*/
	int value = 0;
	int sumblackNum = 0;
	int minvalue = 10000;
	//vector<int>blackNumList;
	for (int j = rect.x; j < rect.width + rect.x; j++)
	{
		blackNum = 0;
		for (int i = rect.y; i < rect.height + rect.y; i++)
		{
			if (img.at<uchar>(i, j) == 0)
			{
				blackNum++;
			}
		}
		sumblackNum += blackNum;
		if (blackNum == 0)
		{
			continue;
		}
		if (blackNum >= 10 && blackNum <= rect.height * 0.4)
		{
			value++;
		}
		if (blackNum < minvalue)
		{
			minvalue = blackNum;
		}
		//blackNumList.push_back(blackNum);
	}
	ratio = sumblackNum * 1.0 / max(rect.height * rect.width - sumblackNum, 1);
	//cout << "ratio = "<< ratio << endl;
	//cout << "ralation = "<< value << endl;
	//cout << "minvalue = " << minvalue << endl;
	if (ratio >= 3.0)
	{
		if (value >= 8) //真正断裂的篦条宽度
		{
			if (value < 15 && minvalue < 10)
			{
				ret = 0;
			}
			else
			{
				ret = 1;
			}
		}
	}
	else if (ratio >= 2.1)
	{
		if (value > 15)
		{
			ret = 1;
			//cout << "yes" << endl;
		}
	}
	else
	{
		if (value > 30)
		{
			ret = 1;
			//cout << "yes" << endl;
		}
	}
	return ret;
}

//int detectQueshi_test1(Mat src, vector<lackInfo> &info, int index)
//{
//	int ret = 0;
//	int height = src.rows;
//	int width = src.cols;
//
//	Mat src_temp = src.clone();
//	threshold(src_temp, src_temp, 50, 255, CV_THRESH_BINARY);
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
//
//	erode(src_temp, src_temp, element);
//	dilate(src_temp, src_temp, element);
//	//还需横向投影，去掉中间的小杂点，要不然会影响下一步找轮廓
//	vector<int> rowValue;
//	getEmptyRow(src_temp, rowValue);
//	for (size_t i = 0; i < rowValue.size(); i++)
//	{
//		int index = 0;
//		index = rowValue[i];
//		for (int j = 0; j < width; j++)
//		{
//			src_temp.at<uchar>(index, j) = 255;
//		}
//	}
//	Mat dstImg_RGB = Mat::zeros(src_temp.size(), CV_8UC3);
//	cvtColor(src_temp, dstImg_RGB, CV_GRAY2BGR);
//	Mat gunImg_temp = src_temp.clone();
//	//图像最外面一圈弄成白色的像素，为了方便后续找轮廓
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			if (i == 0 || i == height - 1 || j <= 5 || j >= width - 5)
//			{
//				{
//					gunImg_temp.at<uchar>(i, j) = 255;
//				}
//			}
//		}
//	}
//	//imshow("thresh", gunImg_temp);
//
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarcy;
//	findContours(gunImg_temp, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
//	vector<RotatedRect> box_gun(contours.size());
//	int contourNum = 0;
//	vector<int>widthList;
//	if (contours.size() > 0)
//	{
//		for (size_t i = 0; i < contours.size(); i++)
//		{
//			int area = 0;
//			area = contourArea(contours[i]);
//			if (contours[i].size() > 5 && area > 1500 && area < 30000)
//			{
//				//cout << "area = " << area << endl;
//				//RotatedRect rect = minAreaRect(contours[i]);
//				RotatedRect rect = fitEllipse(contours[i]);
//				if (rect.size.width < 50)
//				{
//					widthList.push_back(rect.size.width);
//				}
//			}
//		}
//		//cout << "--------" << endl;
//		for (size_t i = 0; i < contours.size(); i++)
//		{
//			//拟合圆
//			int area = 0;
//			area = contourArea(contours[i]);
//			if (contours[i].size() > 5 && area > 1500 && area < 30000)
//			{
//				//cout << "area = " << area << endl;
//				//RotatedRect rect = minAreaRect(contours[i]);
//				RotatedRect rect = fitEllipse(contours[i]);
//				if (rect.size.width > 50)
//				{
//					//ellipse(dstImg_RGB, rect, Scalar(0, 0, 255), 1, CV_AA);
//					RotatedRect rect1 = minAreaRect(contours[i]); //最小外接矩形
//					Point2f P[4];
//					rect1.points(P);
//					Mat rotateImg;
//					Point after_point;
//					int angle_new = 0;
//					if (rect1.angle < -10 && rect1.angle > -45) //角度找的有问题，过滤掉
//					{
//						continue;
//					}
//					if (abs(rect1.angle) < 45 && rect1.angle != 0)
//					{
//						angle_new = int(rect1.angle);
//					}
//					else
//					{
//						angle_new = int(rect1.angle + 90) % 90;
//					}
//					rotateImg = rotateImage(src_temp, angle_new, Point(rect1.center.x, rect1.center.y), after_point);
//
//
//					Rect rect2; //外接矩形
//					rect2.width = max(int(min(rect.size.width, rect.size.height) - 10), 16);
//					rect2.height = max(int(max(rect1.size.width, rect1.size.height) - 20), 16);
//					rect2.x = after_point.x - rect2.width / 2;
//					rect2.y = after_point.y - rect2.height / 2;
//					if (rect2.x <= 0 || rect2.y <= 0)
//					{
//						continue;
//					}
//					if ((rect2.width + rect2.x) >= rotateImg.cols || (rect2.height + rect2.y) >= rotateImg.rows)
//					{
//						continue;
//					}
//					//下一步判断矩形框里的图像黑白像素个数比
//					float ret1 = 0;
//					//ret1 = getPixNumRelation(rotateImg, rect2);
//
//					//waitKey(10);
//					ret1 = getResult(rotateImg, rect2);
//					if (ret1 == 1)
//					{
//						//cout << "ret = " << ret1 << endl;
//						ret = 1;
//						//估计断裂几个篦条
//						lackInfo temp;
//						if (widthList.size() == 0)
//						{
//							temp.num = 1;
//						}
//						else
//						{
//							int meanvalue = 0;
//							for (size_t i1 = 0; i1 < widthList.size(); i1++)
//							{
//								meanvalue += widthList[i1];
//							}
//							meanvalue = meanvalue * 1.0 / widthList.size();
//							meanvalue = max(meanvalue, 21);
//							//cout << "meanvalue = " << meanvalue << endl;
//							temp.num = rect.size.width * 1.0 / meanvalue / 2;
//
//						}
//						temp.rect_loc.width = min(rect1.size.width, rect1.size.height);
//						temp.rect_loc.height = max(rect1.size.width, rect1.size.height);
//						temp.rect_loc.x = max(int(rect1.center.x - temp.rect_loc.width / 2), 0);
//						temp.rect_loc.y = max(int(rect1.center.y - temp.rect_loc.height / 2), 0);
//						info.push_back(temp);
//
//
//
//					}
//					else
//					{
//						ret = 0;
//					}
//				}
//
//				//根据椭圆的短轴的长度来判别是否有缺失
//				//cout << rect.size.width << endl;
//			}
//		}
//	}
//	/*for (size_t i = 0; i < widthList.size(); i++)
//	{
//	cout << widthList[i] << endl;
//	}*/
//	//cout << "-----------------" << endl;
//
//	return info.size();
//}
//int detectQueshi_test(Mat src, vector<lackInfo> &info, int index)
//{
//	int ret = 0;
//	int height = src.rows;
//	int width = src.cols;
//	Mat src_temp = src.clone();
//	threshold(src_temp, src_temp, 50, 255, CV_THRESH_BINARY);
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
//
//	erode(src_temp, src_temp, element);
//	dilate(src_temp, src_temp, element);
//	//还需横向投影，去掉中间的小杂点，要不然会影响下一步找轮廓
//	vector<int> rowValue;
//	getEmptyRow(src_temp, rowValue);
//	for (size_t i = 0; i < rowValue.size(); i++)
//	{
//		int index = 0;
//		index = rowValue[i];
//		for (int j = 0; j < width; j++)
//		{
//			src_temp.at<uchar>(index, j) = 255;
//		}
//	}
//	Mat gunImg_temp = src_temp.clone();
//	//图像最外面一圈弄成白色的像素，为了方便后续找轮廓
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			if (i == 0 || i == height - 1 || j <= 5 || j >= width - 5)
//			{
//				{
//					gunImg_temp.at<uchar>(i, j) = 255;
//				}
//			}
//		}
//	}
//	//imshow("thresh", gunImg_temp);
//
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarcy;
//	findContours(gunImg_temp, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
//	vector<RotatedRect> box_gun(contours.size());
//	int contourNum = 0;
//	//vector<int>widthList;
//	if (contours.size() > 0)
//	{
//		for (size_t i = 0; i < contours.size(); i++)
//		{
//			//拟合圆
//			int area = 0;
//			area = contourArea(contours[i]);
//			if (contours[i].size() > 5 && area > 1500 && area < 30000)
//			{
//				RotatedRect rect = fitEllipse(contours[i]);
//				if (rect.size.width > 50)
//				{
//					RotatedRect rect1 = minAreaRect(contours[i]); //最小外接矩形
//					Point2f P[4];
//					rect1.points(P);
//					Mat rotateImg;
//					Point after_point;
//					int angle_new = 0;
//					if (rect1.angle < -10 && rect1.angle > -45) //角度找的有问题，过滤掉
//					{
//						continue;
//					}
//					if (abs(rect1.angle) < 45 && rect1.angle != 0)
//					{
//						angle_new = int(rect1.angle);
//					}
//					else
//					{
//						angle_new = int(rect1.angle + 90) % 90;
//					}
//					rotateImg = rotateImage(src_temp, angle_new, Point(rect1.center.x, rect1.center.y), after_point);
//
//
//					Rect rect2; //外接矩形
//					rect2.width = max(int(min(rect.size.width, rect.size.height) - 10), 16);
//					rect2.height = max(int(max(rect1.size.width, rect1.size.height) - 20), 16);
//					rect2.x = after_point.x - rect2.width / 2;
//					rect2.y = after_point.y - rect2.height / 2;
//					if (rect2.x <= 0 || rect2.y <= 0)
//					{
//						continue;
//					}
//					if ((rect2.width + rect2.x) >= rotateImg.cols || (rect2.height + rect2.y) >= rotateImg.rows)
//					{
//						continue;
//					}
//					//下一步判断矩形框里的图像黑白像素个数比
//					float ret1 = 0;
//					//ret1 = getPixNumRelation(rotateImg, rect2);
//
//					//waitKey(10);
//					ret1 = getResult(rotateImg, rect2);
//					if (ret1 > 0)
//					{
//						//cout << "ret = " << ret1 << endl;
//						ret = 1;
//						//估计断裂几个篦条
//						lackInfo temp;
//						temp.num = ret1;
//						temp.rect_loc.width = rect1.size.width;
//						temp.rect_loc.height = rect1.size.height;
//						temp.rect_loc.x = max(int(rect1.center.x - temp.rect_loc.width / 2), 0);
//						temp.rect_loc.y = max(int(rect1.center.y - temp.rect_loc.height / 2), 0);
//						info.push_back(temp);
//
//					}
//					else
//					{
//						ret = 0;
//					}
//				}
//
//				//根据椭圆的短轴的长度来判别是否有缺失
//				//cout << rect.size.width << endl;
//			}
//		}
//	}
//	/*for (size_t i = 0; i < widthList.size(); i++)
//	{
//	cout << widthList[i] << endl;
//	}*/
//	//cout << "-----------------" << endl;
//
//	return info.size();
//}
/*
功  能：检测篦板是否存在缺失
输  入：参数1，图像数据（输入）
参数2，篦条缺失位置（输出）
参数3，缺失的篦条个数（输出）
返回值: 1，有缺失
0，无缺失
*/

//int detectQueshi(Mat src, Rect &rect_loc, int &num)
//int detectQueshi(Mat src, vector<lackInfo> &info)
//{
//	int ret = 0;
//	int height = src.rows;
//	int width = src.cols;
//	Mat src_temp = src.clone();
//	threshold(src_temp, src_temp, 50, 255, CV_THRESH_BINARY);
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
//
//	erode(src_temp, src_temp, element);
//	dilate(src_temp, src_temp, element);
//	//还需横向投影，去掉中间的小杂点，要不然会影响下一步找轮廓
//	vector<int> rowValue;
//	getEmptyRow(src_temp, rowValue);
//	for (size_t i = 0; i < rowValue.size(); i++)
//	{
//		int index = 0;
//		index = rowValue[i];
//		for (int j = 0; j < width; j++)
//		{
//			src_temp.at<uchar>(index, j) = 255;
//		}
//	}
//	Mat gunImg_temp = src_temp.clone();
//	//图像最外面一圈弄成白色的像素，为了方便后续找轮廓
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			if (i == 0 || i == height - 1 || j <= 5 || j >= width - 5)
//			{
//				{
//					gunImg_temp.at<uchar>(i, j) = 255;
//				}
//			}
//		}
//	}
//	//imshow("thresh", gunImg_temp);
//
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarcy;
//	findContours(gunImg_temp, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
//	vector<RotatedRect> box_gun(contours.size());
//	int contourNum = 0;
//	//vector<int>widthList;
//	if (contours.size() > 0)
//	{
//		for (size_t i = 0; i < contours.size(); i++)
//		{
//			//拟合圆
//			int area = 0;
//			area = contourArea(contours[i]);
//			if (contours[i].size() > 5 && area > 1500 && area < 30000)
//			{
//				RotatedRect rect = fitEllipse(contours[i]);
//				if (rect.size.width > 50)
//				{
//					RotatedRect rect1 = minAreaRect(contours[i]); //最小外接矩形
//					Point2f P[4];
//					rect1.points(P);
//					Mat rotateImg;
//					Point after_point;
//					int angle_new = 0;
//					if (rect1.angle < -10 && rect1.angle > -45) //角度找的有问题，过滤掉
//					{
//						continue;
//					}
//					if (abs(rect1.angle) < 45 && rect1.angle != 0)
//					{
//						angle_new = int(rect1.angle);
//					}
//					else
//					{
//						angle_new = int(rect1.angle + 90) % 90;
//					}
//					rotateImg = rotateImage(src_temp, angle_new, Point(rect1.center.x, rect1.center.y), after_point);
//
//
//					Rect rect2; //外接矩形
//					rect2.width = max(int(min(rect.size.width, rect.size.height) - 10), 16);
//					rect2.height = max(int(max(rect1.size.width, rect1.size.height) - 20), 16);
//					rect2.x = after_point.x - rect2.width / 2;
//					rect2.y = after_point.y - rect2.height / 2;
//					if (rect2.x <= 0 || rect2.y <= 0)
//					{
//						continue;
//					}
//					if ((rect2.width + rect2.x) >= rotateImg.cols || (rect2.height + rect2.y) >= rotateImg.rows)
//					{
//						continue;
//					}
//					//下一步判断矩形框里的图像黑白像素个数比
//					float ret1 = 0;
//					//ret1 = getPixNumRelation(rotateImg, rect2);
//
//					//waitKey(10);
//					ret1 = getResult(rotateImg, rect2);
//					if (ret1 > 0)
//					{
//						//cout << "ret = " << ret1 << endl;
//						ret = 1;
//						//估计断裂几个篦条
//						lackInfo temp;
//						temp.num = ret1;
//						temp.rect_loc.width = rect1.size.width;
//						temp.rect_loc.height = rect1.size.height;
//						temp.rect_loc.x = max(int(rect1.center.x - temp.rect_loc.width / 2), 0);
//						temp.rect_loc.y = max(int(rect1.center.y - temp.rect_loc.height / 2), 0);
//						info.push_back(temp);
//
//
//					}
//					else
//					{
//						ret = 0;
//					}
//				}
//
//				//根据椭圆的短轴的长度来判别是否有缺失
//				//cout << rect.size.width << endl;
//			}
//		}
//	}
//	/*for (size_t i = 0; i < widthList.size(); i++)
//	{
//	cout << widthList[i] << endl;
//	}*/
//	//cout << "-----------------" << endl;
//
//
//	return info.size();
//}


/*
功能：检测壁板是否归位
输入:图像数据（输入）
返回值：0，表示未归位
1，表示归位
*/
int detectGuiwei(Mat src) 
{


	int ret = 1;
	//找一个阈值，进行分割
	Mat src_temp = src.clone();
	int height = src_temp.rows;
	int width = src_temp.cols;
	//应该是有2套参数
	int effectiveArea = 0;
	int effectiveWidth = 0;
	int effectiveWidth2 = 0;
	int linenum = 0;
	int effectiveHeight = 0;
	if (height < 40) //ROI是上面的最小区域
	{
		effectiveArea = 70;
		effectiveWidth = 7;
		effectiveWidth2 = 18;
		linenum = 1;
		effectiveHeight = height - linenum * 2;
	}
	else if (height < 120) //ROI是小区域
	{
		effectiveArea = 180;
		effectiveWidth = 15;
		effectiveWidth2 = 37;
		linenum = 2;
		effectiveHeight = height - linenum * 2;

	}
	else //ROI是大区域
	{
		effectiveArea = 1000;
		effectiveWidth = 22;
		effectiveWidth2 = 45;
		linenum = 3;
		effectiveHeight = height - linenum * 2;
	}

	//threshold(src_temp, src_temp, 128, 255, CV_THRESH_OTSU);
	//256个，范围是0，255.
	//imshow("src_temp",src_temp);
	//waitKey(100);
	cv::MatND histogram;
	const int histSize = 256;
	float range[] = { 0, 256 }; //这个地方256不能写错，不能写成255，写错就打错特错了
	const float *ranges[] = { range };
	const int channels = 0;
	calcHist(&src_temp, 1, &channels, cv::Mat(), histogram, 1, &histSize, &ranges[0], true, false);
	int row = histogram.rows;
	int col = histogram.cols;
	int threshvalue = 80;
	float *h = (float*)histogram.data;
	float ratioThresh = 0.5;
	float sum = 0;
	int s = 0;
	for (int i = 255; i >= 0; i--)
	{
		if (h[i] > 0)
		{
			//cout << "h[i] = " << h[i] / (height * width) << endl;
			sum += h[i] / (height * width);
			//cout << "i = " << i << "---," << h[i] <<",f[i] = " << h[i] / (height * width) << endl;
			s += h[i];
			//cout << s<<"-----,"<<sum << endl;
			if (sum >= ratioThresh)
			{
				threshvalue = max(1, i - 1);
				//cout << "threshvalue = " << threshvalue << endl;
				break;
			}


		}
	}

	//灰度均值
	Mat means, stddev;
	meanStdDev(src_temp, means, stddev);
	//threshvalue = max(80.0,means.at<double>(0) - 10);
	//threshvalue = means.at<double>(0);
	//int threshvalue = 10;
	//cout << "threshValue_means = " << means.at<double>(0) << endl;

	if (threshvalue < 100 || means.at<double>(0) < 100)  //这个值的设定与ratioThresh有关系，成反比
	{
		ret = 1;
		return ret;
	}
	if (threshvalue > 120)
	{
		threshvalue = means.at<double>(0);
	}
	threshold(src_temp, src_temp, threshvalue, 255, CV_THRESH_BINARY_INV);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	erode(src_temp, src_temp, element, Point(-1, -1), 2); //目的是把那种未恢复到正确状态，但是未完全把黑占满
	dilate(src_temp, src_temp, element);

	/*Mat struct1 = getStructuringElement(MORPH_RECT, Size(effectiveWidth / 2,1 ), Point(-1, -1));
	erode(src_temp, src_temp, struct1, Point(-1, -1));
	dilate(src_temp, src_temp, struct1, Point(-1, -1));*/

	//imshow("thresh", src_temp);
	//waitKey(10);

	Mat src_temp1 = src_temp.clone();
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(src_temp1, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	vector<RotatedRect> box_gun(contours.size());
	int contourNum = 0;
	vector<int>widthList;
	Mat dstImg_RGB = Mat::zeros(src_temp.size(), CV_8UC3);
	cvtColor(src_temp, dstImg_RGB, CV_GRAY2BGR);
	if (contours.size() > 0)
	{
		for (size_t i = 0; i < contours.size(); i++)
		{
			//拟合圆
			int area = 0;
			area = contourArea(contours[i]);
			//cout << "area = " << area << endl;
			if (contours[i].size() > 5 && area > effectiveArea && area < 30000)
			{
				Rect rect = boundingRect(contours[i]);
				cv::rectangle(dstImg_RGB, rect, Scalar(0, 0, 255), 0);
				//imshow("rotate", dstImg_RGB);
				int width_new = rect.width;
				//cout << "new_width = " << width_new << endl;
				float ratio = 0;
				ratio = area * 1.0 / (rect.width * rect.height);
				//cout << "area ratio = " << ratio << endl;
				if (ratio <= 0.6)
				{
					continue;
				}
				if (width_new >= effectiveWidth && width_new <= effectiveWidth2)//合理的区域
				{
					float ratio1 = 0;
					ratio1 = rect.height * 1.0 / effectiveHeight;
					//cout << "ratio1 = " << ratio1 << endl;
					if (ratio1 <= 0.63 && ratio1 >= 0.3)
					{
						ret = 0;
						//cout << "-------------no" << endl;
						//imshow("rotate", dstImg_RGB);
						//waitKey(0);
						break;
					}

				}

			}
		}
	}
	return ret;
}
int getSobelImg(Mat src, Mat &sobelImg)
{
	int ret = 0;
	Mat grad_x;
	Mat grad_y;
	Mat abs_grad_x;
	Mat abs_grad_y;
	Sobel(src, grad_x, src.depth(), 0, 1, 3);
	Sobel(src, grad_y, src.depth(), 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobelImg); //一种近似的估计
	return ret;
}
//检测篦条的磨损程度
int detectMosun(Mat src, lackInfo &info)
{
	int ret = 0;
	float ratio = 0;
	int defectNum = 0;
	int height = src.rows;
	int width = src.cols;
	Mat src_temp = src.clone();
	//Mat sobelImg;
	//getSobelImg(src_temp, sobelImg);
	//threshold(sobelImg, sobelImg, 100, 255, CV_THRESH_BINARY);

	threshold(src_temp, src_temp, 50, 255, CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的

	erode(src_temp, src_temp, element);
	dilate(src_temp, src_temp, element);
	//imshow("before", src_temp);

	Mat element1 = getStructuringElement(MORPH_RECT, Size(1, 10)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的

	dilate(src_temp, src_temp, element1);
	erode(src_temp, src_temp, element1);

	//还需横向投影，去掉中间的小杂点，要不然会影响下一步找轮廓
	vector<int> rowValue;
	getEmptyRow(src_temp, rowValue);
	for (size_t i = 0; i < rowValue.size(); i++)
	{
		int index = 0;
		index = rowValue[i];
		for (int j = 0; j < width; j++)
		{
			src_temp.at<uchar>(index, j) = 255;
			//sobelImg.at<uchar>(index, j) = 0;
		}
	}
	//threshold(src_temp, src_temp, 50, 255, CV_THRESH_BINARY_INV);
	//imshow("sobel", sobelImg);
	//waitKey(0);
	Mat gunImg_temp = src_temp.clone();
	//图像最外面一圈弄成白色的像素，为了方便后续找轮廓
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1 || j <= 5 || j >= width - 5)
			{
				{
					gunImg_temp.at<uchar>(i, j) = 255;
				}
			}
		}
	}
#ifdef showResult
	imshow("thresh", gunImg_temp);
#endif
	//waitKey(200);
	//return 0;
#ifdef showResult
	Mat dstImg_RGB1 = Mat::zeros(src.size(), CV_8UC3);
	cvtColor(src, dstImg_RGB1, CV_GRAY2BGR);
#endif
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(gunImg_temp, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	//imshow("test2",gunImg_temp);
	vector<RotatedRect> box_gun(contours.size());
	int contourNum = 0;
	//vector<int>widthList;
	//imshow("src_temp1", src_temp);
	//waitKey(0);
	if (contours.size() > 0)
	{
		Mat dstImg_RGB2 = Mat::zeros(src.size(), CV_8UC3);
		cvtColor(src, dstImg_RGB2, CV_GRAY2BGR);
		//imshow("rotate", dstImg_RGB2);

		for (size_t i = 0; i < contours.size(); i++)
		{
			//拟合圆
			int area = 0;
			area = contourArea(contours[i]);

			if (contours[i].size() > 5 && area > 1500 && area < 30000)
			{
				//RotatedRect rect = fitEllipse(contours[i]); 
				RotatedRect rect = minAreaRect(contours[i]);
				//正常一个篦条的空隙是22，大于22的空隙就可能存在磨损.此处设定为25
				int normalWidth = 25;
				int normalHeight = 110;
				int rect_width = min(rect.size.width, rect.size.height);
				int rect_height = max(rect.size.width, rect.size.height);
				if (rect_width > normalWidth && rect_height >= normalHeight)
				{
#if 1
					//ellipse(dstImg_RGB2, rect, Scalar(0, 255, 0), 1, CV_AA);
					//Point2f pts[4];
					//rect.points(pts);//确定旋转矩阵的四个顶点
					//for (int i = 0; i < 4; i++) {
					//	line(dstImg_RGB2, pts[i], pts[(i + 1) % 4], Scalar(0, 255, 0), 2);
					//}
					//imshow("test",dstImg_RGB2);
					//waitKey(0);
					if (rect_width < 50) //单个空隙
					{
						//取当前篦条的左边和当前
						Rect leftRect;
						leftRect.x = max(int(rect.center.x - 60), 0);
						leftRect.y = max(int(rect.center.y - rect_height / 2), 0);
						leftRect.height = min(int(rect.center.y + rect_height / 2), height - 1) - leftRect.y;
						leftRect.width = min(int(rect.center.x + rect_width / 2), width - 1) - leftRect.x;
						//rectangle(dstImg_RGB2, leftRect, Scalar(0, 0, 255), 1, CV_AA);
						Mat subImg = src_temp(leftRect).clone();
#ifdef showResult
						imshow("subImg", subImg);
#endif
						////threshold(subImg, subImg,128,255,CV_THRESH_BINARY_INV);
						//IplImage subImg1 = subImg;
						//FillInternalContours(&subImg1, 100);
						////threshold(subImg, subImg, 128, 255, CV_THRESH_BINARY_INV);
						//imshow("subImg1", subImg);

						//先确定是否磨损严重(腐蚀后，是否连通)，如果磨损严重，判断是否是小球阴影导致的误检
						Mat subImg_temp = subImg.clone();
						Mat element = getStructuringElement(MORPH_RECT, Size(10, 1));
						dilate(subImg_temp, subImg_temp, element);
						erode(subImg_temp, subImg_temp, element, Point(-1, -1), 2); //针对小球的情况很有效
																					//imshow("subImg_after", subImg_temp);

						int begin_y = 10;
						int end_y = subImg_temp.rows - 10;
						int lineNo = 0;
						for (int i = begin_y; i < end_y; i++)
						{
							int symbol = 0;
							for (int j = 0; j < subImg_temp.cols; j++)
							{
								int value = 0;
								value = subImg_temp.at<uchar>(i, j);
								if (value == 255)
								{
									symbol = 1;
									break;
								}
							}
							if (symbol == 0)
							{
								lineNo = i;
								break;
							}
						}
						if (lineNo > 0 && leftRect.x > 40) //第二个条件是为了预防误认为左边的空隙
						{
#ifdef showResult
							rectangle(dstImg_RGB2, leftRect, Scalar(0, 0, 255), 1, CV_AA);
							imshow("colorRect", dstImg_RGB2);
#endif
							defectRegion t;
							t.rect_loc = leftRect;
							t.type = 0;
							info.rectInfo.push_back(t);
							//waitKey(0);
							defectNum += 1;
							ratio += (leftRect.height * leftRect.width);
							ret = 1;
						}
					}
					else
					{
						//有连通区，可能存在断裂，看一下是不是小球导致的，另外确定一下断裂的像素个数

						Rect Rect1;
						Rect1.x = max(int(rect.center.x - rect_width / 2), 0);
						Rect1.y = max(int(rect.center.y - rect_height / 2), 0);
						Rect1.height = rect_height;
						Rect1.width = rect_width;
						Mat subImg = src_temp(Rect1).clone();
#ifdef showResult
						imshow("thresh", subImg);
#endif
						//cout << "subImg.area = " << Rect1.width * Rect1.height << endl;

						//先判断当前断裂是否是大于20个像素的
						Mat subImg1 = subImg.clone();
						Mat element = getStructuringElement(MORPH_RECT, Size(1, 15));
						dilate(subImg1, subImg1, element);
						erode(subImg1, subImg1, element, Point(-1, -1), 1); //针对小球的情况很有效

																			//周围一圈变白
																			//图像最外面一圈弄成白色的像素，为了方便后续找轮廓
						for (int i = 0; i < Rect1.height; i++)
						{
							for (int j = 0; j < Rect1.width; j++)
							{
								if (i <= 2 || i >= Rect1.height - 2 || j <= 2 || j >= Rect1.width - 2)
								{
									{
										subImg1.at<uchar>(i, j) = 255;
									}
								}
							}
						}
						//imshow("subImg1", subImg1);

						//找轮廓，分析宽度
						Mat subImg2 = subImg1.clone();
						//imwrite("test.bmp",subImg1);
						Mat subImgColor;
						cvtColor(subImg2, subImgColor, CV_GRAY2BGR);
						vector<vector<Point>> contoursSubImg;
						vector<Vec4i> hierarcysubImg;
						findContours(subImg2, contoursSubImg, hierarcysubImg, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

						vector<RotatedRect> box(contoursSubImg.size());
						if (contoursSubImg.size() > 0)
						{
							for (size_t i = 0; i < contoursSubImg.size(); i++)
							{
								//拟合圆
								int area = 0;
								area = contourArea(contoursSubImg[i]);
								//cout << "area = " << area << endl;
								if (contoursSubImg[i].size() > 5 && area > 1500 && area < Rect1.width * Rect1.height * 0.9)
								{
									RotatedRect rect = minAreaRect(contoursSubImg[i]);
									int rect_width = min(rect.size.width, rect.size.height);
									int rect_height = max(rect.size.width, rect.size.height);

									if (rect_height >= normalHeight) //正常高度，看宽度
									{
										if (rect_width >= normalWidth * 2 + 5)
										{
											/*	Rect rectShow;
											rectShow.x = max(int(rect.center.x - rect_width / 2), 0);
											rectShow.y = max(int(rect.center.y - rect_height / 2), 0);
											rectShow.height = rect_height;
											rectShow.width = rect_width;*/

											//Point2f pts[4];
											//rect.points(pts);//确定旋转矩阵的四个顶点
											//for (int i = 0; i < 4; i++) {
											//line(subImgColor, pts[i], pts[(i + 1) % 4], Scalar(0, 255, 0), 2);
											//}
											//imshow("colorRect_test", subImgColor);
											//waitKey(100);

											//确定断裂点，然后确定断裂点左和右是否存在小球
											//旋转
											Mat rotateImg;
											Point after_point;
											int angle_new = 0;
											if (rect.angle < -10 && rect.angle > -45) //角度找的有问题，过滤掉
											{
												continue;
											}
											if (abs(rect.angle) < 45 && rect.angle != 0)
											{
												angle_new = int(rect.angle);
											}
											else
											{
												angle_new = int(rect.angle + 90) % 90;
											}
											rotateImg = rotateImage(subImg1, angle_new, Point(rect.center.x, rect.center.y), after_point);
											//imshow("rotateImg", rotateImg);
											//imwrite("rotateImg.bmp",rotateImg);
											Rect rect2; //外接矩形
											rect2.width = min(rect.size.width, rect.size.height);
											rect2.height = max(rect.size.width, rect.size.height);
											rect2.x = after_point.x - rect2.width / 2;
											rect2.y = after_point.y - rect2.height / 2;
											if (rect2.x <= 0 || rect2.y <= 0)
											{
												continue;
											}
											if ((rect2.width + rect2.x) >= rotateImg.cols || (rect2.height + rect2.y) >= rotateImg.rows)
											{
												continue;
											}
											int nBreakNum = 0;
											nBreakNum = getResult(rotateImg, rect2); //必须缺失得大于20个像素
											if (nBreakNum > 0) //有断裂(缺失) //返回值就是断裂的篦条个数
											{
#ifdef showResult
												cout << "yes has breakpoint,Num = " << nBreakNum << endl;
#endif
												//估计一个断裂个数，计算一个ratio
												nBreakNum = max(nBreakNum, int(Rect1.width / 55.0 + 0.5));
												defectNum += nBreakNum;
												ratio += (Rect1.height * Rect1.width);
												defectRegion t;
												t.rect_loc = Rect1;
												t.type = 1;
												info.rectInfo.push_back(t);
#ifdef showResult
												rectangle(dstImg_RGB2, Rect1, Scalar(255, 0, 0), 1, CV_AA);
#endif
												ret = 1;
											}
#ifdef showResult
											imshow("subImg1", subImg1);
#endif
											//waitKey(0);
										}
									}
								}
							}
						}



					}


#else
					RotatedRect rect1 = minAreaRect(contours[i]); //最小外接矩形
					Point2f P[4];
					rect1.points(P);
					Mat rotateImg;
					Point after_point;
					int angle_new = 0;
					if (rect1.angle < -10 && rect1.angle > -45) //角度找的有问题，过滤掉
					{
						continue;
					}
					if (abs(rect1.angle) < 45 && rect1.angle != 0)
					{
						angle_new = int(rect1.angle);
					}
					else
					{
						angle_new = int(rect1.angle + 90) % 90;
					}
					rotateImg = rotateImage(src_temp, angle_new, Point(rect1.center.x, rect1.center.y), after_point);


					Rect rect2; //外接矩形
					rect2.width = max(int(min(rect.size.width, rect.size.height) - 10), 16);
					rect2.height = max(int(max(rect1.size.width, rect1.size.height) - 20), 16);
					rect2.x = after_point.x - rect2.width / 2;
					rect2.y = after_point.y - rect2.height / 2;
					if (rect2.x <= 0 || rect2.y <= 0)
					{
						continue;
					}
					if ((rect2.width + rect2.x) >= rotateImg.cols || (rect2.height + rect2.y) >= rotateImg.rows)
					{
						continue;
					}
					//下一步判断矩形框里的图像黑白像素个数比
					float ret1 = 0;
					//ret1 = getPixNumRelation(rotateImg, rect2);
#ifdef showResult
					Mat dstImg_RGB2 = Mat::zeros(rotateImg.size(), CV_8UC3);
					cvtColor(rotateImg, dstImg_RGB2, CV_GRAY2BGR);
					rectangle(dstImg_RGB2, rect2, Scalar(0, 255, 0), 1, CV_AA);
					imshow("rotate", dstImg_RGB2);
#endif
					//waitKey(10);
					ret1 = getResult(rotateImg, rect2);
					if (ret1 > 0)
					{
						//cout << "ret = " << ret1 << endl;
						ret = 1;
						//估计断裂几个篦条
						lackInfo temp;
						temp.num = ret1;
						temp.rect_loc.width = rect1.size.width;
						temp.rect_loc.height = rect1.size.height;
						temp.rect_loc.x = max(int(rect1.center.x - temp.rect_loc.width / 2), 0);
						temp.rect_loc.y = max(int(rect1.center.y - temp.rect_loc.height / 2), 0);
						info.push_back(temp);

#ifdef showResult
						rectangle(dstImg_RGB1, rect2, Scalar(0, 255, 0), 1, CV_AA);
#endif
					}
					else
					{
						ret = 0;
					}
#endif
				}

				//根据椭圆的短轴的长度来判别是否有缺失
				//cout << rect.size.width << endl;
			}
		}
#ifdef showResult
		imshow("colorRect", dstImg_RGB2);
#endif
	}
	if (ret > 0)
	{
		info.nDefectNum = defectNum;
		int t = 0;
		t = ratio * 1.0 / (height * width) * 1000;
		ratio = t / 1000.0;
		info.ratio = ratio;
	}
#ifdef showResult
	waitKey(10);
#endif
	return ret;
}