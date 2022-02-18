#pragma once

#include "ImageUtility.h"
#include "opencv.hpp"
// CMsgImg 对话框

class CMsgImg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgImg)

public:
	CMsgImg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgImg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MSGIMG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	std::string CString2String(CString cs);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	cv::Mat m_Mat_Img;
	string strImg;
	void UpdateImg();
	afx_msg void OnBnClickedOk();
	void UpdateImg1();
};
