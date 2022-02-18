#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "vector"

#pragma once


// CImageShow 对话框

class CImageShow : public CDialogEx
{
	DECLARE_DYNAMIC(CImageShow)

public:
	CImageShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageShow();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMAGESHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	BOOL ShowMatToWnd(CWnd *pWnd, cv::Mat& img);
	std::vector<std::string> findFile(std::string dir);
	std::vector<int> HKSeekChehao(std::vector<std::string> path, int chehao);
	std::vector<int> THSeekChehao(std::vector<std::string> path, int chehao);
	BOOL HKCompareIP(std::string path, std::string ip);
	BOOL THCompareIP(std::string path, std::string ip);
	std::string CString2String(CString cs);
	CString INTtoIP(uint32_t num);
	CString ReturnPath();
};

