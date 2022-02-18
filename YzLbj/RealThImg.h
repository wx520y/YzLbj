#pragma once

#include "opencv.hpp"
#include "ImageUtility.h"
#include"TextSpeaker.h"
// CRealThImg 对话框
#define WM_SPEAK WM_USER + 100
class CRealThImg : public CDialogEx
{
	DECLARE_DYNAMIC(CRealThImg)

public:
	CRealThImg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRealThImg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REALTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void UpdateImg();
	cv::Mat m_Mat_Img;
	afx_msg void OnPaint();
	std::string CString2String(CString cs);
	gds::CTextSpeaker m_TextSpeak;
	CString str_SpeakerText;
	CString str_Title;
	void Speak();
	void StopSpeak();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetSpeaker(MsgList slist);
private:
	CFont m_CFont_Title;
	afx_msg LRESULT OnSpeak(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
