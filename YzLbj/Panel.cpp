// Panel.cpp : 实现文件
//

#include "stdafx.h"
#include "YzLbj.h"
#include "Panel.h"
#include "afxdialogex.h"
#include "YzLbjDlg.h"
extern CYzLbjDlg* g_pMainDlg;
extern MsgList msgList_Sql;
extern CString g_ImgPath;
extern BOOL zanting;
// CPanel 对话框

IMPLEMENT_DYNAMIC(CPanel, CDialogEx)

CPanel::CPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONTROL, pParent)
	, m_BOOL_LockIng(FALSE)
{

}

CPanel::~CPanel()
{
}

void CPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_LOCKIMG, m_BOOL_LockIng);
}


BEGIN_MESSAGE_MAP(CPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_LOCKIMG, &CPanel::OnBnClickedCheckLockimg)
	ON_BN_CLICKED(IDCANCEL, &CPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPanel 消息处理程序


void CPanel::OnBnClickedOk()
{
	//g_pMainDlg->m_CEvent_EventThPro.SetEvent();
	//for (int i = 0; i < 30; i++)
	//{
	//	MsgList l;
	//	//g_pMainDlg->m_List_Msg.InsertItem(1, _T("gggggg"));
	//	//int nRow = g_pMainDlg->m_List_Msg.InsertItem(0, _T("fgfg"));// 插入行
	//	COleDateTime t = COleDateTime::GetCurrentTime();
	//	CString str = t.Format(_T("%Y-%m_%d_%H_%M_%S"));
	//	//g_pMainDlg->m_List_Msg.SetItemText(nRow, 1, str);// 设置其它列数据
	//	//g_pMainDlg->m_List_Msg.SetItemText(nRow, 2, _T("2"));// 设置其它列数据
	//	//g_pMainDlg->m_List_Msg.SetItemText(nRow, 3, _T("6"));// 设置其它列数据
	//	l.msg_xuhao = i;
	//	l.msg_time = t.m_dt;
	//	l.msg_chehao = 2;
	//	l.mgs_bibanhao = 6;
	//	l.msg_leixng = GRATE_Lost;
	//	CString str11 = _T("D:\\1.bmp");
	//	g_pMainDlg->SendMessage(WM_UPDATELIST, (WPARAM)(&l), (LPARAM)(&str11));
	//}
	/*COleDateTime tm = COleDateTime::GetCurrentTime();
	MsgList msgList;
	msgList.msg_time = tm.m_dt;
	msgList.mgs_bibanhao = 5;
	msgList.msg_chehao = 4;
	msgList.msg_leixng = GRATE_NotReset;
	msgList.msg_xuhao = 22;
	CString s = _T("D:\\bbb\\IMG_20141227_115348.jpg");
	g_pMainDlg->m_CriticalSection_Sql.Lock();
	msgList_Sql = msgList;
	g_ImgPath = s;
	g_pMainDlg->m_CriticalSection_Sql.Unlock();
	g_pMainDlg->m_CEvent_EventWriteSql.SetEvent();*/
	/*m_BOOL_LockIng = TRUE;
	g_pMainDlg->m_BOOL_ChkLockImg = TRUE;
	g_pMainDlg->UpdateData(FALSE);
	zanting = TRUE;
	g_pMainDlg->On32777();
	UpdateData(FALSE);*/
	
}


void CPanel::OnBnClickedCheckLockimg()
{
	AfxMessageBox(_T("修改ROI后要重启程序，否则小轴计数会有问题！！！！"));
	UpdateData(TRUE);
	g_pMainDlg->m_BOOL_ChkLockImg = m_BOOL_LockIng;
	g_pMainDlg->UpdateData(FALSE);
	if (!m_BOOL_LockIng)
	{
		zanting = m_BOOL_LockIng;
	}
	g_pMainDlg->OnBnClickedCheckLockimg();
	// TODO: 在此添加控件通知处理程序代码
}


void CPanel::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BOOL_LockIng = FALSE;
	g_pMainDlg->m_BOOL_ChkLockImg = FALSE;
	g_pMainDlg->UpdateData(FALSE);
	zanting = FALSE;
	//g_pMainDlg->On32777();
	UpdateData(FALSE);
	CDialogEx::OnCancel();
}
