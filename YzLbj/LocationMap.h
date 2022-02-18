#pragma once
#include "afxcmn.h"
#include "ExListCtrl.h"
#include "ImageShow.h"
#include "PictureEx.h"
#include "afxwin.h"
#include "PassWordDlg.h"
// CLocationMap 对话框
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

//增加导出excel功能
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
class CLocationMap : public CDialogEx
{
	DECLARE_DYNAMIC(CLocationMap)

public:
	CLocationMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLocationMap();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POSMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
	
protected:
	// m_CListCtrl;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GetRect(CDC * dc, CPoint lt, CPoint lb, CPoint rt, CPoint rb, int R, int r);
	void ShowNumbers(CDC *dc, int num, int height, CRect rect_t[],  CRect pic_rect_t[]);
	void DrawFlashLight(CDC *dc,BOOL  SolidOrNot);
	void ShowMatToWnd(CDC *hDc, CRect drect, cv::Mat& img);
	static const int num = 20;
	static const int StepNum = 10;
	int getindex();
	BOOL  flashflag;
	BOOL  rectflag;
	BOOL  m_Login;
	CRect rect[num], rect1[num];
	CRect pic_rect[num], pic_rect1[num];
	CImageShow* pImageshow;
	CPassWordDlg* pPassWordDlg;
private:
	int  start_Th;
	int  start_HK;
	int  index;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void PaintCircle(CDC * cdc, BOOL state, CRect rect);
	CPictureEx m_gif;
	void DrawlineArrow(CPoint p1, CPoint p2, double theta, int length);
	afx_msg void OnBnClickedButton1();
	CListCtrl m_CListCtrl;
	void DoGetUsersData(CString starttime, CString endtime, int Chehao, int BiBanHao);
	void DODeleteData(int CheHao, int BiBanHao);
	afx_msg void OnBnClickedButton2();
	void InitMsgList(BOOL isInitFir, CListCtrl  &m_List_Msg);
	afx_msg void OnEnChangeEditChehao();
	int m_CheHao;
	int m_BiBanHao;
	afx_msg void OnEnChangeEditBibanhao();
	afx_msg void OnBnClickedButtonClearwarn();
	void DelReplyWithPosition(POSITION pos);
	afx_msg void OnNMDblclkListMsg2(NMHDR *pNMHDR, LRESULT *pResult);
	std::string CString2String(CString cs);
	afx_msg void OnNMCustomdrawListMsg2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLogin();
	void ChangeLoginState();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int  step;
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static unsigned int __stdcall ThreadProc_WriteExcel(LPVOID  lpParam);
	static unsigned int __stdcall ThreadProc_QueryDB(LPVOID  lpParam);
	CEvent  m_Event_WriteExcel;
	HANDLE m_hTProc_WriteExcel;
	CEvent  m_Event_QueryDB;
	HANDLE m_hTProc_QueryDB;
	void WriteExcel();
	void QueryDB();
};
