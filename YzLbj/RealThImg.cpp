// RealThImg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YzLbj.h"
#include "RealThImg.h"
#include "afxdialogex.h"
#include "YzLbjDlg.h"

extern CYzLbjDlg* g_pMainDlg;
// CRealThImg �Ի���

IMPLEMENT_DYNAMIC(CRealThImg, CDialogEx)

CRealThImg::CRealThImg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REALTH, pParent)
{
	str_SpeakerText = _T("");
	m_CFont_Title.CreatePointFont(180, _T("����"));
}

CRealThImg::~CRealThImg()
{
	m_CFont_Title.DeleteObject();
}

void CRealThImg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRealThImg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRealThImg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CRealThImg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SPEAK,&CRealThImg::OnSpeak)
END_MESSAGE_MAP()


// CRealThImg ��Ϣ�������


void CRealThImg::OnBnClickedOk()
{
	Speak();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}
void CRealThImg::UpdateImg()
{
	CRect rt;
	GetDlgItem(IDC_STATIC_REALTHIMG)->GetWindowRect(rt);
	InvalidateRect(rt, FALSE);
}

void CRealThImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	if (!m_Mat_Img.empty())
	{
		ImageUtility::DisplayImage(m_Mat_Img, IDC_STATIC_REALTHIMG, GetSafeHwnd());
	}
}
std::string CRealThImg::CString2String(CString cs)
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

void CRealThImg::Speak()
{
	m_TextSpeak.Speak(str_SpeakerText);
	//m_TextSpeak.m_tts->WaitUntilDone(1000 * 10);
	//m_TextSpeak.Speak(_T("m_TextSpeak.Speak(str_SpeakerText);m_TextSpeak.Speak(str_SpeakerText);m_TextSpeak.Speak(str_SpeakerText);m_TextSpeak.Speak(str_SpeakerText);m_TextSpeak.Speak(str_SpeakerText);"));
}

void CRealThImg::StopSpeak()
{
	m_TextSpeak.Speak(NULL);
}


void CRealThImg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(0);
	StopSpeak();
	CDialogEx::OnCancel();
}


void CRealThImg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (0 == nIDEvent)
	{
		PostMessage(WM_SPEAK,0,0);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CRealThImg::SetSpeaker(MsgList slist)
{
	MsgList l = slist;
	COleDateTime t(l.msg_time);
	CString str_t = t.Format(_T("%Y��%m��%d�� %Hʱ%M��%S��"));
	switch (l.msg_leixng)
	{
	case GRATE_NotReset:
		str_SpeakerText.Format(_T("��ע�� %dС�� ��%d���� %s"),l.msg_chehao,l.mgs_bibanhao,_T("δ��λ"));
		str_Title.Format(_T("%s %dС�� ��%d���� %s"), str_t,l.msg_chehao, l.mgs_bibanhao, _T("δ��λ"));
		break;
	case GRATE_Lost:
		str_SpeakerText.Format(_T("��ע�� %dС�� ��%d���� %s"), l.msg_chehao, l.mgs_bibanhao, _T("����ȱʧ"));
		str_Title.Format(_T("%s %dС�� ��%d���� %s"), str_t, l.msg_chehao, l.mgs_bibanhao, _T("����ȱʧ"));
		break;
	case GRATE_LostandNotReset:
		str_SpeakerText.Format(_T("��ע�� %dС�� ��%d���� %s"), l.msg_chehao, l.mgs_bibanhao, _T("����ȱʧ+δ��λ"));
		str_Title.Format(_T("%s %dС�� ��%d���� %s"), str_t, l.msg_chehao, l.mgs_bibanhao, _T("����ȱʧ+δ��λ"));
		break;
	default:
		break;
	}
	
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(str_Title);
	Speak();
	int le = str_SpeakerText.GetLength();
	int g = (int)(60.0 / CHARPERMINTER * le/*+ 0.5*/) * 1000;
	SetTimer(0, g, NULL);
}


LRESULT CRealThImg::OnSpeak(WPARAM wParam, LPARAM lParam)
{
	//StopSpeak();
	Speak();
	return LRESULT();
}

BOOL CRealThImg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	GetDlgItem(IDC_STATIC_TITLE)->SetFont(&m_CFont_Title);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
