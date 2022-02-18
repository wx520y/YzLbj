#pragma once

#include "ImageUtility.h"
#include "opencv.hpp"
// CMsgImg �Ի���

class CMsgImg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgImg)

public:
	CMsgImg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgImg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MSGIMG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
