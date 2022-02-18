// MsgImg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YzLbj.h"
#include "MsgImg.h"
#include "afxdialogex.h"
#include "YzLbjDlg.h"

// CMsgImg �Ի���
extern CYzLbjDlg* g_pMainDlg;
IMPLEMENT_DYNAMIC(CMsgImg, CDialogEx)

CMsgImg::CMsgImg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MSGIMG, pParent)
{

}

CMsgImg::~CMsgImg()
{
}

void CMsgImg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgImg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CMsgImg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMsgImg ��Ϣ�������

std::string CMsgImg::CString2String(CString cs)
{

	int n = cs.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, cs, cs.GetLength(), NULL, 0, NULL, NULL);
	char* pTest = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, cs, cs.GetLength(), pTest, len, NULL, NULL);
	pTest[len] = '\0';
	std::string s = pTest;
	delete[] pTest;
	return s;
}
BOOL CMsgImg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMsgImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	if (!m_Mat_Img.empty())
	{
		ImageUtility::DisplayImage(m_Mat_Img, IDC_STATIC_MSGIMG, GetSafeHwnd());
	}
	//m_Mat_Img = cv::
}

void CMsgImg::UpdateImg()
{
	if (g_pMainDlg->m_int_msgSlt >= 0 && g_pMainDlg->m_CStringA_msgTmgPath.GetSize() > 0)
	{
		string strImg = CString2String(g_pMainDlg->m_CStringA_msgTmgPath.GetAt(g_pMainDlg->m_int_msgSlt));
		m_Mat_Img = cv::imread(strImg, IMREAD_COLOR);
		CRect rt;
		GetDlgItem(IDC_STATIC_MSGIMG)->GetWindowRect(rt);
		InvalidateRect(rt, FALSE);
	}
}
void CMsgImg::UpdateImg1()
{
	if (g_pMainDlg->m_int_msgSlt >= 0 && g_pMainDlg->m_CStringA_msgTmgPath.GetSize() > 0)
	{
		
		CRect rt;
		GetDlgItem(IDC_STATIC_MSGIMG)->GetWindowRect(rt);
		InvalidateRect(rt, FALSE);
	}
}

void CMsgImg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
