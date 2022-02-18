// PassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YzLbj.h"
#include "PassWordDlg.h"
#include "afxdialogex.h"
#include "LocationMap.h"

// CPassWordDlg 对话框
CPassWordDlg* pPassWordDlg;
extern CLocationMap* pLocationMap;
IMPLEMENT_DYNAMIC(CPassWordDlg, CDialogEx)

CPassWordDlg::CPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PASSWORD_DIALOG, pParent)
{
	pPassWordDlg = this;
}

CPassWordDlg::~CPassWordDlg()
{
}

void CPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_mima);
	DDX_Text(pDX, IDC_EDIT3, m_laomima);
	DDX_Text(pDX, IDC_EDIT4, m_xinmima);
	DDX_Text(pDX, IDC_EDIT5, m_querenxinmima);
}


BEGIN_MESSAGE_MAP(CPassWordDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPassWordDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPassWordDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CPassWordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassWordDlg 消息处理程序
BOOL CPassWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

									// TODO: 在此添加额外的初始化代码
	CRect rectDivide;
	GetWindowRect(&RectLarge);
	GetDlgItem(IDC_DIVIDE)->GetWindowRect(&rectDivide);
	RectSmall.left = RectLarge.left;
	RectSmall.right = RectLarge.right;
	RectSmall.top = RectLarge.top;
	RectSmall.bottom = rectDivide.bottom;

	SetWindowPos(NULL, 0, 0, RectSmall.Width(), RectSmall.Height(), SWP_NOMOVE | SWP_NOZORDER);




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}







void CPassWordDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	if (GetDlgItemText(IDC_BUTTON1, str), str == _T("更改密码↓"))
	{
		SetWindowPos(NULL, 0, 0, RectLarge.Width(), RectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BUTTON1, _T("更改密码↑"));

	}
	else
	{

		SetWindowPos(NULL, 0, 0, RectSmall.Width(), RectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BUTTON1, _T("更改密码↓"));
	}
	/*if (GetDlgItemText(IDC_BUTTON1, str), str == "更改密码↓")
	{
	SetWindowPos(NULL, 0, 0, RectSmall.Width(), RectSmall.Height(),
	SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
	SetWindowPos(NULL, 0, 0, RectLarge.Width(), RectLarge.Height(),
	SWP_NOMOVE | SWP_NOZORDER);
	}*/
}


void CPassWordDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	UpdateData();

	::GetPrivateProfileString(_T("PSW"), _T("password"), _T("sante888"), str.GetBuffer(100), 10, theApp.str_PublicIniPath);
	if (m_mima == str)
	{
		//MessageBox(_T("密码正确！"));
		//GetParent()->SetDlgItemTextW(IDC_BUTTON_LOGIN, _T("注销"));
		pLocationMap->SetDlgItemTextW(IDC_BUTTON_LOGIN, _T("注销"));
		pLocationMap->m_Login = TRUE;
		SetDlgItemText(IDC_EDIT1, _T(""));
		OnCancel();
	}
	else
	{
		MessageBox(_T("密码错误！"));
		
	}
	str.ReleaseBuffer();
	return;
}


void CPassWordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CString str;
	::GetPrivateProfileString(_T("PSW"), _T("password"), _T("sante888"), str.GetBuffer(100), 10, theApp.str_PublicIniPath);
	if (str == m_laomima)
	{
		if (m_xinmima == m_querenxinmima)
		{
			::WritePrivateProfileString(_T("PSW"), _T("password"),  m_xinmima, theApp.str_PublicIniPath);
			MessageBox(_T("密码修改成功！"));
			m_laomima = _T("");
			m_xinmima = _T("");
			m_querenxinmima = _T("");
			UpdateData(false);
		}
		else
		{
			MessageBox(_T("请重新确认密码！"), _T("提示"));
			m_laomima = _T("");
			m_xinmima = _T("");
			m_querenxinmima = _T("");
			UpdateData(false);
		}
	}
	else
	{
		MessageBox(_T("请输入正确的密码！"), _T("提示"));
		m_laomima = _T("");
		m_xinmima = _T("");
		m_querenxinmima = _T("");
		UpdateData(false);

	}
	str.ReleaseBuffer();
}
