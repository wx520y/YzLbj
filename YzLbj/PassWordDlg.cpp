// PassWordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YzLbj.h"
#include "PassWordDlg.h"
#include "afxdialogex.h"
#include "LocationMap.h"

// CPassWordDlg �Ի���
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


// CPassWordDlg ��Ϣ�������
BOOL CPassWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

									// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rectDivide;
	GetWindowRect(&RectLarge);
	GetDlgItem(IDC_DIVIDE)->GetWindowRect(&rectDivide);
	RectSmall.left = RectLarge.left;
	RectSmall.right = RectLarge.right;
	RectSmall.top = RectLarge.top;
	RectSmall.bottom = rectDivide.bottom;

	SetWindowPos(NULL, 0, 0, RectSmall.Width(), RectSmall.Height(), SWP_NOMOVE | SWP_NOZORDER);




	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}







void CPassWordDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	if (GetDlgItemText(IDC_BUTTON1, str), str == _T("���������"))
	{
		SetWindowPos(NULL, 0, 0, RectLarge.Width(), RectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BUTTON1, _T("���������"));

	}
	else
	{

		SetWindowPos(NULL, 0, 0, RectSmall.Width(), RectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BUTTON1, _T("���������"));
	}
	/*if (GetDlgItemText(IDC_BUTTON1, str), str == "���������")
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	UpdateData();

	::GetPrivateProfileString(_T("PSW"), _T("password"), _T("sante888"), str.GetBuffer(100), 10, theApp.str_PublicIniPath);
	if (m_mima == str)
	{
		//MessageBox(_T("������ȷ��"));
		//GetParent()->SetDlgItemTextW(IDC_BUTTON_LOGIN, _T("ע��"));
		pLocationMap->SetDlgItemTextW(IDC_BUTTON_LOGIN, _T("ע��"));
		pLocationMap->m_Login = TRUE;
		SetDlgItemText(IDC_EDIT1, _T(""));
		OnCancel();
	}
	else
	{
		MessageBox(_T("�������"));
		
	}
	str.ReleaseBuffer();
	return;
}


void CPassWordDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CString str;
	::GetPrivateProfileString(_T("PSW"), _T("password"), _T("sante888"), str.GetBuffer(100), 10, theApp.str_PublicIniPath);
	if (str == m_laomima)
	{
		if (m_xinmima == m_querenxinmima)
		{
			::WritePrivateProfileString(_T("PSW"), _T("password"),  m_xinmima, theApp.str_PublicIniPath);
			MessageBox(_T("�����޸ĳɹ���"));
			m_laomima = _T("");
			m_xinmima = _T("");
			m_querenxinmima = _T("");
			UpdateData(false);
		}
		else
		{
			MessageBox(_T("������ȷ�����룡"), _T("��ʾ"));
			m_laomima = _T("");
			m_xinmima = _T("");
			m_querenxinmima = _T("");
			UpdateData(false);
		}
	}
	else
	{
		MessageBox(_T("��������ȷ�����룡"), _T("��ʾ"));
		m_laomima = _T("");
		m_xinmima = _T("");
		m_querenxinmima = _T("");
		UpdateData(false);

	}
	str.ReleaseBuffer();
}
