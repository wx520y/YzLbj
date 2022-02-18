// LocationMap.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YzLbj.h"
#include "LocationMap.h"
#include "afxdialogex.h"
#include "YzLbjDlg.h"

using namespace cv;
extern CYzLbjDlg* g_pMainDlg;
//extern CPassWordDlg* pPassWordDlg;
// CLocationMap �Ի���
CLocationMap* pLocationMap;

IMPLEMENT_DYNAMIC(CLocationMap, CDialogEx)
#define COLOR_DEFAULT 0 //Ĭ����ɫ

#define COLOR_RED 1 //��ɫ
#define COLOR_ORANGE 2 //��ɫ
#define COLOR_BLUE 3 //��ɫ
#define COLOR_YELLOW 4 //��ɫ
#define COLOR_GRAY 5 //��ɫ

Mat img = imread("zhoutou.jpg");


CLocationMap::CLocationMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_POSMAP, pParent)
	,pImageshow(NULL)
	,pPassWordDlg(NULL)
	,start_Th(1)
	,start_HK(260)
	,index(0)
	,flashflag(FALSE)
	,rectflag(TRUE)
	,m_Login(FALSE)
	, m_CheHao(0)
	, m_BiBanHao(0)
	,step(0)
{
	if (pImageshow == NULL)
	{
		pImageshow = new CImageShow;
		pImageshow->Create(IDD_DIALOG_IMAGESHOW, this);
	}
	if (pPassWordDlg == NULL)
	{
		pPassWordDlg = new CPassWordDlg;
		pPassWordDlg->Create(IDD_PASSWORD_DIALOG, this);
	}
	/*if (m_gif == NULL)
	{
			m_gif= new CPictureEx;
			m_gif->Create(_T(""), WS_VISIBLE , CRect(200, 100, 1200, 300), NULL,12345);
	}*/
	pLocationMap = this;
	

}


CLocationMap::~CLocationMap()
{
	if (pImageshow != NULL)
	{
		pImageshow->DestroyWindow();
		delete pImageshow;
		pImageshow = NULL;
	}
	if (pPassWordDlg != NULL)
	{
		pPassWordDlg->DestroyWindow();
		delete pPassWordDlg;
		pPassWordDlg = NULL;
	}
	if (m_hTProc_WriteExcel)
	{
		m_Event_WriteExcel.SetEvent();
		WaitForSingleObject(m_hTProc_WriteExcel, 1000);
		CloseHandle(m_hTProc_WriteExcel);
		m_hTProc_WriteExcel = NULL;
	}	
	if (m_hTProc_QueryDB)
	{
	//m_Event_QueryDB.SetEvent();
	//WaitForSingleObject(m_hTProc_QueryDB, 1000);
	CloseHandle(m_hTProc_QueryDB);
	m_hTProc_QueryDB = NULL;
	}
	/////////�رճ���ʱ�����ڴ�����
	/*if (m_hTProc_QueryDB)
	{
		m_Event_QueryDB.SetEvent();
		WaitForSingleObject(m_hTProc_QueryDB, 1000);
		CloseHandle(m_hTProc_QueryDB);
		m_hTProc_QueryDB = NULL;
	}*/

	KillTimer(1);
	KillTimer(2);
}

void CLocationMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_CListCtrl);
	DDX_Control(pDX, IDC_STATIC_GIF, m_gif);
	DDX_Control(pDX, IDC_LIST_MSG2, m_CListCtrl);
	DDX_Text(pDX, IDC_EDIT_CHEHAO, m_CheHao);
	DDV_MinMaxInt(pDX, m_CheHao, 1, 449);
	DDX_Text(pDX, IDC_EDIT_BIBANHAO, m_BiBanHao);
	DDV_MinMaxInt(pDX, m_BiBanHao, 1, 10);
}


BEGIN_MESSAGE_MAP(CLocationMap, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CLocationMap::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON1, &CLocationMap::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLocationMap::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT_CHEHAO, &CLocationMap::OnEnChangeEditChehao)
	ON_EN_CHANGE(IDC_EDIT_BIBANHAO, &CLocationMap::OnEnChangeEditBibanhao)
	ON_BN_CLICKED(IDC_BUTTON_CLEARWARN, &CLocationMap::OnBnClickedButtonClearwarn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MSG2, &CLocationMap::OnNMDblclkListMsg2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MSG2, &CLocationMap::OnNMCustomdrawListMsg2)
ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLocationMap::OnBnClickedButtonLogin)
//ON_WM_ERASEBKGND()
//ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLocationMap ��Ϣ�������


void CLocationMap::OnPaint()
{
	
	//CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	//int hh = 6;
	//for (int i = 0; i < 10; i++)
	//{
	//	for (int j = 0;j < 449;j++)
	//	{
	//		int l = hh*j;
	//		int r = l + hh;
	//		int t = hh * i;
	//		int b = t + hh;
	//		CRect rr(l, t, r, b);
	//		dc.Ellipse(rr);
	//		//dc.Draw3dRect(rr, RGB(255, 0, 0), RGB(0, 255, 0));
	//	}
	//}
	///////////////////*******************************ͼ������˸����Ϊ˫����20200707*****************************//////////////////////////////////
	//CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	//CRect   rect1;
	//GetClientRect(&rect1);
	//CDC   dcMem;
	//dcMem.CreateCompatibleDC(&dc);
	//CBitmap   bmpBackground;
	//bmpBackground.LoadBitmap(IDB_BITMAP1);
	////IDB_BITMAP1���Լ���ͼ��Ӧ��ID 
	//BITMAP   bitmap;
	//bmpBackground.GetBitmap(&bitmap);
	//CBitmap   *pbmpOld = dcMem.SelectObject(&bmpBackground);
	//dc.SetStretchBltMode(HALFTONE);
	//dc.StretchBlt(0, 0, rect1.Width(), rect1.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	//CDC *cdc;
	//cdc = new CClientDC(this);
	//int R = 100, r = 36;
	//int ltx = 400, lty = 200;//��ʾ����ε����ϵ�
	//int lbx = 400, lby = 400;
	//int rtx = 1500, rty = 200;
	//int rbx = 1500, rby = 400;
	//CPoint lt(ltx, lty);
	//CPoint lb(lbx, lby);
	//CPoint rt(rtx, rty);
	//CPoint rb(rbx, rby);
	//GetRect(cdc, lt, lb, rt, rb, R, r);
	//CPoint  arrow_left1((ltx + rtx) / 2 - 100, lty - 100);
	//CPoint  arrow_right1((ltx + rtx) / 2 + 100, lty - 100);
	//CPoint  arrow_left2((lbx + rbx) / 2 + 100, lby + 100);
	//CPoint  arrow_right2((lbx + rbx) / 2 - 100, lby + 100);
	//DrawlineArrow(arrow_left1, arrow_right1, 30, 20);
	//DrawlineArrow(arrow_left2, arrow_right2, 30, 20);
	//SetTimer(1, 1000 * 2.5, NULL);
	//ReleaseDC(cdc);
	///////////////////*******************************ͼ������˸����Ϊ˫����20200707*****************************//////////////////////////////////
	//CPoint  arrow_HKpos1((rect[9].left+ rect[9].right) / 2 , (rect[9].top + rect[9].bottom) / 2 - r-50);
	//CPoint  arrow_HKpos2((rect[9].left + rect[9].right) / 2, (rect[9].top + rect[9].bottom) / 2 - r );
	//CPoint  arrow_Thpos1((rect[10].left + rect[10].right) / 2, (rect[10].top + rect[10].bottom) / 2 - r - 50);
	//CPoint  arrow_Thpos2((rect[10].left + rect[10].right) / 2, (rect[10].top + rect[10].bottom) / 2 - r );
	//DrawlineArrow(arrow_HKpos1, arrow_HKpos2,30,10);
	//DrawlineArrow(arrow_Thpos1, arrow_Thpos2,30,10);
	//CFont newfont;
	//newfont.CreateFont(
	//	20, //����ĸ߶�
	//	10,  //����Ŀ��
	//	0,		//������ʾ�ĽǶ�
	//	0,		//����ĽǶ�
	//	FW_NORMAL,//����İ���
	//	false, //б������
	//	false, //���»��ߵ�����
	//	0, //��ɾ���ߵ�����
	//	ANSI_CHARSET, //������ַ���
	//	OUT_DEFAULT_PRECIS, //����ľ���
	//	CLIP_DEFAULT_PRECIS, //�ü��ľ���
	//	DEFAULT_QUALITY, //�߼�����������豸��ʵ��
	//					 //����֮��ľ���
	//	DEFAULT_PITCH, //����������弯
	//	_T("����")//��������
	//);
	//CFont *oldfont = cdc->SelectObject(&newfont);
	//SetTextAlign(cdc->GetSafeHdc(), TA_CENTER);
	//SetBkMode(cdc->GetSafeHdc(), TRANSPARENT);
	//cdc->SetTextColor(RGB(255,0,0));
	//CString  str;
	//str.Format(_T("����������λ��"));
	//cdc->TextOut((rect[10].left + rect[10].right) / 2, (rect[10].top + rect[10].bottom) / 2 - r - 70, str);
	//str.Format(_T("�����Ǽ��λ��"));
	//cdc->TextOut((rect[9].left + rect[9].right) / 2, (rect[9].top + rect[9].bottom) / 2 - r - 70, str);
	//DrawFlashLight(cdc, flashflag);
	
	//DeleteObject(pbmpOld);
	
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	CRect   rect11;
	GetClientRect(&rect11);
	CDC   dcMem, dcMem_Bk;
	dcMem.CreateCompatibleDC(&dc);
	dcMem_Bk.CreateCompatibleDC(&dc);
	CBitmap   bmpBackground;




	bmpBackground.LoadBitmap(IDB_BITMAP1);
	//IDB_BITMAP1���Լ���ͼ��Ӧ��ID 
	BITMAP   bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap   *pbmpOld = dcMem_Bk.SelectObject(&bmpBackground);
	//dc.SetStretchBltMode(HALFTONE);
	//dc.StretchBlt(0, 0, rect11.Width(), rect11.Height(), &dcMem_Bk, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	//dcMem.StretchBlt(0, 0, rect11.Width(), rect11.Height(), &dcMem_Bk, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	//dcMem.BitBlt(0, 0, rect11.Width(), rect11.Height(), &dcMem_Bk, 0, 0,  SRCCOPY);
	CBitmap tmp;
	tmp.CreateCompatibleBitmap(&dc, rect11.Width(), rect11.Height());
	dcMem.SelectObject(&tmp);
	tmp.DeleteObject();
	dcMem.BitBlt(0, 0, rect11.Width(), rect11.Height(), &dcMem_Bk, 0, 0, SRCCOPY);
	CDC *cdc;
	//cdc = new CClientDC(this);
	/*cdc == GetDC();
	
	if (!cdc)
		return;*/
	cdc = &dcMem;
	int R = 100, r = 36;
	int ltx = 400, lty = 200;//��ʾ����ε����ϵ�
	int lbx = 400, lby = 400;
	int rtx = 1500, rty = 200;
	int rbx = 1500, rby = 400;
	CPoint lt(ltx, lty);
	CPoint lb(lbx, lby);
	CPoint rt(rtx, rty);
	CPoint rb(rbx, rby);
	GetRect(cdc, lt, lb, rt, rb, R, r);
	CPoint  arrow_left1((ltx + rtx) / 2 - 100, lty - 100);
	CPoint  arrow_right1((ltx + rtx) / 2 + 100, lty - 100);
	CPoint  arrow_left2((lbx + rbx) / 2 + 100, lby + 100);
	CPoint  arrow_right2((lbx + rbx) / 2 - 100, lby + 100);
	//DrawlineArrow(arrow_left1, arrow_right1, 30, 20);
	//DrawlineArrow(arrow_left2, arrow_right2, 30, 20);
	ShowNumbers(cdc, 20, 30, rect, pic_rect);
	DrawFlashLight(cdc, flashflag);
	//SetTimer(1, 1000 , NULL);
	
	dc.BitBlt(0, 0, rect11.Width(), rect11.Height(), &dcMem, 0, 0, SRCCOPY);
	
	//dcMem_Bk.DeleteDC();
	//dcMem.DeleteDC();
	ReleaseDC(cdc);
	//CDialogEx::OnPaint();
}


BOOL CLocationMap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	/*int g_iCurScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	CRect r(20, 0, g_iCurScreenWidth, g_iCurScreenHeight / 2);
	ClientToScreen(r);
	SetWindowPos(NULL, 0, 0, g_iCurScreenWidth, g_iCurScreenHeight / 2, SWP_NOZORDER);
	MoveWindow(0, 0, g_iCurScreenWidth, g_iCurScreenHeight);*/
	m_CListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // ����ѡ��������
	InitMsgList(TRUE, m_CListCtrl);
	//for (int i = 0; i < 4; i++)
	//{
	//	CString str;
	//	str.Format(_T("%d"), i);
	//	m_CListCtrl.InsertColumn(i, str, LVCFMT_LEFT, 100);
	//}
	//CString strName, strAge, strSex;
	//for (int i = 0; i <= 7; i++) {

	//	strName.Format(_T("Сʯͷ%d"), i);

	//	strAge.Format(_T("%d"), 20 + i);

	//	strSex = i % 2 ? _T("��") : _T("Ů");

	//	m_CListCtrl.InsertItem(i, _T("")); // ������

	//	m_CListCtrl.SetItemText(i, 1, strName); // ���õ�2��(����)

	//	m_CListCtrl.SetItemText(i, 2, strAge); // ���õ�3��(����)

	//	m_CListCtrl.SetItemText(i, 3, strSex); // ���õ�4��(�Ա�)

	//}

	//const RECT *  gifrect;
	//CRect gifrect;
	//GetDlgItem(12345)->GetWindowRect(gifrect);
	//ScreenToClient(gifrect);//��ȡ��ʾ��1
	//m_gif->SetPaintRect(&gifrect);
	//m_gif->Load(_T("C:\\Users\\87482\\Desktop\\1.gif"));
	//m_gif->Draw();

	//CRect gifrect;
	//GetDlgItem(IDC_STATIC_GIF)->GetWindowRect(gifrect);
	//ScreenToClient(gifrect);//��ȡ��ʾ��1
	//MoveWindow(0, 200, 1500, 200, true);
	
	CRect   gif{ 0, 0, 1100, 200 };
	//GetDlgItem(IDC_STATIC_GIF)->GetWindowRect(gif);
	m_gif.Load(_T("111.gif"));
	m_gif.SetPaintRect(&gif);
	m_gif.Draw();
	GetDlgItem(IDC_STATIC_GIF)->ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_STATIC_GIF)->SetWindowPos(NULL, 400, 200, 1100, 200, SWP_SHOWWINDOW);
	/*CFont cfont;
	cfont.CreatePointFont(12, _T("����"), NULL);
	GetDlgItem(IDC_STATIC_BIBANHAO)->SetFont(&cfont);
	GetDlgItem(IDC_STATIC_CHEHAO)->SetFont(&cfont);*/
	CString str;
	str.Format(_T("����ȱʧ����% 3d"), 0);
	//GetDlgItem(IDC_STATIC_LOST)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_STATIC_LOST, str);
	str = _T("");
	SetDlgItemText(IDC_STATIC_THSTATE, str);
	SetTimer(1, 500, NULL);
	SetTimer(2, 1000, NULL);
	SetTimer(3, 1000*60, NULL);
	m_hTProc_WriteExcel= (HANDLE)_beginthreadex(NULL, 0, CLocationMap::ThreadProc_WriteExcel, this, 0, NULL);
	m_hTProc_QueryDB = (HANDLE)_beginthreadex(NULL, 0, CLocationMap::ThreadProc_QueryDB, this, 0, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLocationMap::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	//CDialogEx::OnOK();
}
void CLocationMap::GetRect(CDC * cdc,CPoint lt, CPoint lb, CPoint rt, CPoint rb, int R,int r)
{
	CPen *pen = new CPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen *oldpen;
	oldpen =cdc->SelectObject(pen);
	
	//CPoint lt(ltx, lty);
	//CPoint lb(lbx, lby);
	//CPoint rt(rtx, rty);
	//CPoint rb(rbx, rby);
	int ltx = lt.x;
	int lty = lt.y;
	int lbx = lb.x;
	int lby = lb.y;
	int rtx = rt.x;
	int rty = rt.y;
	int rbx = rb.x;
	int rby = rb.y;
	cdc->MoveTo(lb);
	cdc->LineTo(rb);
	cdc->Arc(rbx - R, rty, rbx + R, rby, rbx, rby, rtx, rty);
	cdc->MoveTo(rt);
	cdc->LineTo(lt);
	cdc->Arc(ltx - R, lty, ltx + R, rby, ltx, lty, lbx, lby);
	/*hdc->MoveTo(lb);
	hdc->LineTo(rb);
	hdc->Arc(rbx-R,rty,rbx+R,rby,rbx,rby,rtx,rty);
	hdc->MoveTo(rt);
	hdc->LineTo(lt);
	hdc->Arc(ltx-R,lty,ltx+R,rby,ltx,lty,lbx,lby);*/
	int  const n = 4;
	int  pic_r = r*1.414 / 2;
	CPoint center[num], lefttop[num], rightbuttom[num];
	CPoint pic_lefttop[num], pic_rightbuttom[num];
	CPoint center1[num], lefttop1[num], rightbuttom1[num];
	CPoint pic_lefttop1[num], pic_rightbuttom1[num];
	/*if (rectflag)
	{*/
		//center[0] = CPoint(rbx - 6 * r - 3 * r - 3 * r / 2, rby + r);
		//center[1] = CPoint(rbx - 4 * r - 2 * r - 2 * r / 2, rby + r);
		//center[2] = CPoint(rbx - 2 * r - r - r / 2, rby + r);
		//for (int i = 0; i < n + 1; i++)
		//{
		//	double theta = i*CV_PI / n;
		//	center[i + 3] = CPoint((int)(rbx + (R + r)*sin(theta)), (int)((rby + rty) / 2 + (R + r)*cos(theta)));
		//	//lefttop[i]=CPoint(center[i].x-r,center[i].y-r);
		//	//rightbuttom[i]=CPoint(center[i].x+r,center[i].y+r);
		//}
		//center[8] = CPoint(rtx - 2 * r - r - r / 2, rty - r);
		//center[9] = CPoint(rtx - 4 * r - 2 * r - 2 * r / 2, rty - r);
		//center[10] = CPoint(ltx + 6 * r + 3 * r + 3 * r / 2, lty - r);
		//center[11] = CPoint(ltx + 4 * r + 2 * r + 2 * r / 2, lty - r);
		//center[12] = CPoint(ltx + 2 * r + r + r / 2, lty - r);
		//for (int i = 0; i < n + 1; i++)
		//{
		//	double theta = i*CV_PI / n;
		//	center[i + 13] = CPoint((int)(lbx - (R + r)*sin(theta)), (int)((lby + lty) / 2 - (R + r)*cos(theta)));
		//}
		//center[18] = CPoint(lbx + 2 * r + r + r / 2, lby + r);
		//center[19] = CPoint(lbx + 4 * r + 2 * r + 2 * r / 2, lby + r);



		//for (int i = 0; i < num; i++)
		//{
		//	lefttop[i] = CPoint(center[i].x - r, center[i].y - r);
		//	rightbuttom[i] = CPoint(center[i].x + r, center[i].y + r);
		//	rect[i] = CRect(lefttop[i], rightbuttom[i]);
		//	cdc->Ellipse(rect[i]);
		//	pic_lefttop[i] = CPoint(center[i].x - pic_r, center[i].y - pic_r);
		//	pic_rightbuttom[i] = CPoint(center[i].x + pic_r, center[i].y + pic_r);
		//	pic_rect[i] = CRect(pic_lefttop[i], pic_rightbuttom[i]);


		//}
		
	/*}
	else 
	{*/
		center[0] = CPoint(rbx - 6 * r - 3 * r - 3 * r / 2  - step * 7 * r / 2 / StepNum, rby + r);//������λ���
		center[1] = CPoint(rbx - 4 * r - 2 * r - 2 * r / 2  - step * 7 * r / 2 / StepNum, rby + r);
		center[2] = CPoint(rbx - 2 * r - r - r / 2  - step * 7 * r / 2 / StepNum, rby + r);
		center[3] = CPoint(rbx - step * 7 * r / 2 / StepNum, rby + r);
		for (int i = 1; i < n + 1; i++)
		{
			double theta = i*CV_PI / n;
			center[i + 3] = CPoint((int)(rbx + (R + r)*sin(theta - step*CV_PI / 4/StepNum)), (int)((rby + rty) / 2 + (R + r)*cos(theta - step*CV_PI / 4 / StepNum)));
			//lefttop[i]=CPoint(center[i].x-r,center[i].y-r);
			//rightbuttom[i]=CPoint(center[i].x+r,center[i].y+r);
		}

		center[8] = CPoint(rtx - 2 * r - r - r / 2 + step * 7 * r / 2 / StepNum, rty - r);
		center[9] = CPoint(rtx - 4 * r - 2 * r - 2 * r / 2 + step * 7 * r / 2 / StepNum, rty - r);
		center[10] = CPoint(ltx + 6 * r + 3 * r + 3 * r / 2 + step * 7 * r / 2 / StepNum, lty - r);
		center[11] = CPoint(ltx + 4 * r + 2 * r + 2 * r / 2 + step * 7 * r / 2 / StepNum, lty - r);
		center[12] = CPoint(ltx + 2 * r + r + r / 2 + step * 7 * r / 2 / StepNum, lty - r);
		center[13] = CPoint(ltx + step * 7 * r / 2 / StepNum, lty - r);
		for (int i = 1; i < n + 1; i++)
		{
			double theta = i*CV_PI / n;
			center[i + 13] = CPoint((int)(lbx - (R + r)*sin(theta - step*CV_PI / 4 / StepNum)), (int)((lby + lty) / 2 - (R + r)*cos(theta - step*CV_PI / 4 / StepNum)));
		}
		//center1[17] = CPoint(lbx - step * 7 * r / 2 / StepNum, lby + r);
		center[18] = CPoint(lbx + 2 * r + r + r / 2 - step * 7 * r / 2 / StepNum, lby + r);
		center[19] = CPoint(lbx + 4 * r + 2 * r + 2 * r / 2 - step * 7 * r / 2 / StepNum, lby + r);
		for (int i = 0; i < num; i++)
		{
			lefttop[i] = CPoint(center[i].x - r, center[i].y - r);
			rightbuttom[i] = CPoint(center[i].x + r, center[i].y + r);
			rect[i] = CRect(lefttop[i], rightbuttom[i]);
			cdc->Ellipse(rect[i]);
			pic_lefttop[i] = CPoint(center[i].x - pic_r, center[i].y - pic_r);
			pic_rightbuttom[i] = CPoint(center[i].x + pic_r, center[i].y + pic_r);
			pic_rect[i] = CRect(pic_lefttop[i], pic_rightbuttom[i]);
		}

		
	//}
	cdc->SelectObject(oldpen);
	delete pen;
	pen = NULL;

}

void CLocationMap::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
	{
		if (g_pMainDlg->RunOrStop)
		{
			step++;
			if (step%StepNum == 0)
			{
				step = 0;
			}
			if (!m_gif.IsPlaying())
			{
				m_gif.Draw();
			}
			
			//CPen *pen = new CPen(PS_SOLID, 2, RGB(255, 0, 0));
			//CPen *oldpen;

			//++start_Th;
			//++start_HK;
			//g_pMainDlg->SetGrateNumDelta(1);
			
		}
		else
		{
			step = 0;
			m_gif.Stop();
		}
		start_Th = g_pMainDlg->GetGrateNum(theApp.Th_CHeHaoBc - 1);
		start_HK = g_pMainDlg->GetGrateNum(theApp.Hk_CHeHaoBc - 10);
		if (start_Th > GRATENUM) start_Th = start_Th - GRATENUM;
		if (start_HK > GRATENUM) start_HK = start_HK - GRATENUM;
		//oldpen = hdc->SelectObject(pen);
		/*if (rectflag)
		{*/
			/*oldpen = hdc->SelectObject(pen);
			
			for (int i=0;i<num;i++)
			{
				hdc->Ellipse(rect[i]);
			}
			hdc->SelectObject(oldpen);*/
			/*for (int i = 0; i<num; i++)
			{
				InvalidateRect(rect1[i]);

			}*/
		
			//OnPaint();
		
			//ShowNumbers(hdc, 20, 30, rect, pic_rect);
			//rectflag = FALSE;
			
		/*}
		else
		{*/
			/*oldpen = hdc->SelectObject(pen);
			for (int i = 0; i<num; i++)
			{
				hdc->Ellipse(rect1[i]);
			}
			hdc->SelectObject(oldpen);*/
			/*for (int i = 0; i < num; i++)
			{
				InvalidateRect(rect[i]);

			}*/
			//OnPaint();
			//ShowNumbers(hdc, 20, 30, rect1,pic_rect1);
			//rectflag = TRUE;
			//g_pMainDlg->SetGrateNumDelta(1);
		//}
		
		//ShowNumbers(hdc, 20, 30, rect1, pic_rect1);
		//Invalidate();
		//hdc->SelectObject(oldpen);
		//if (flashflag)
		//{
		//	//DrawFlashLight(hdc, flashflag);
		//	flashflag = FALSE;
		//} 
		//else
		//{
		//	//DrawFlashLight(hdc, flashflag);
		//	flashflag = true;
		//}
		flashflag = ~flashflag;	
		InvalidateRect(CRect(200,100,1700,500),FALSE);
		
		//Invalidate(FALSE);
		//Invalidate();
		//ReleaseDC(hdc);
		//delete pen;
		//pen = NULL;
		//delete hdc;
		//hdc = NULL;
		break;
		//default:
		//break;
	}
	case 2:
	{
		 int  LostNum = 0;
		 for (int i = 0; i < GRATENUM; i++)
		 {

			 for (int j = 0; j < 10; j++)
			 {
				 if (g_pMainDlg->m_int_GrateState[i][j] == 1)
					 LostNum++;
			 }
		 }
		
		CString str;
		str.Format(_T("����ȱʧ����% 3d"), LostNum);
		//GetDlgItem(IDC_STATIC_LOST)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC_LOST, str);
		//GetDlgItem(IDC_STATIC_LOST)->ShowWindow(SW_NORMAL);
		//GetDlgItem(IDC_STATIC_LOST)->GetParent()->RedrawWindow();
		CRect  rt;
		GetDlgItem(IDC_STATIC_LOST)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		InvalidateRect(rt, FALSE);
		CString th_state[THCAMNUM];
		for (int i=0;i<THCAMNUM;i++)
		{
			if (g_pMainDlg->m_Th_State[i] == TRUE)
			{
				th_state[i].Format(_T("�����"));
			}
			else
			{
				th_state[i].Format(_T("���˳�"));
			}
		}
		
		str.Format(_T("�������ϣ�%s\n�����Ǳ���%s"), th_state[0], th_state[1]);
		SetDlgItemText(IDC_STATIC_THSTATE, str);
		CRect rt2;
		GetDlgItem(IDC_STATIC_THSTATE)->GetWindowRect(&rt2);
		ScreenToClient(&rt2);
		InvalidateRect(rt2, FALSE);
		break;
	}
	case 3:
	{
		g_pMainDlg->RunOrStop = TRUE;
		break;
	}
   }
  
	CDialogEx::OnTimer(nIDEvent);
}
void CLocationMap::ShowNumbers(CDC *cdc, int num, int height, CRect rect_t[],CRect pic_rect_t[])
{

	
	/*CPen pen, pen1, *oldpen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pen1.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	oldpen = cdc->SelectObject(&pen);*/
	for (int i = 0; i < num; i++)
	{
		CString str;
		
		if (i < num / 2)
		{

			if (i + start_Th < GRATENUM)
			{
				str.Format(_T("%03d"),  i + start_Th+1);
				PaintCircle(cdc, g_pMainDlg->BibanState[ i + start_Th], rect_t[i]);
			}
			else {
				str.Format(_T("%03d"), i + start_Th - GRATENUM+1);
				PaintCircle(cdc, g_pMainDlg->BibanState[i + start_Th - GRATENUM], rect_t[i]);//�������������״̬
			}
		}
		else {

			if (i +  start_HK< GRATENUM)
			{
				str.Format(_T("%03d"), start_HK +i+1);
				PaintCircle(cdc, g_pMainDlg->BibanState[start_HK +i ], rect_t[i]);
			}
			else {
				str.Format(_T("%03d"), start_HK + i- GRATENUM+1);
				PaintCircle(cdc, g_pMainDlg->BibanState[start_HK +i- GRATENUM], rect_t[i]);
			}
		}
		ShowMatToWnd(cdc,pic_rect_t[i], img);
		//str.Format(_T("%03d"),i+1);
		int length = str.GetLength();
		int width = height / length * 4 / 5;
		//int height=r*2/length*4/5;
		CFont newfont;
		newfont.CreateFont(
			height, //����ĸ߶�
			width,  //����Ŀ��
			0,		//������ʾ�ĽǶ�
			0,		//����ĽǶ�
			FW_NORMAL,//����İ���
			false, //б������
			false, //���»��ߵ�����
			0, //��ɾ���ߵ�����
			ANSI_CHARSET, //������ַ���
			OUT_DEFAULT_PRECIS, //����ľ���
			CLIP_DEFAULT_PRECIS, //�ü��ľ���
			DEFAULT_QUALITY, //�߼�����������豸��ʵ��
							 //����֮��ľ���
			DEFAULT_PITCH, //����������弯
			_T("����")//��������
		);
		CFont *oldfont = cdc->SelectObject(&newfont);
		SetTextAlign(cdc->GetSafeHdc(), TA_CENTER);
		//CSize size=dc.GetTextExtent(str);
		//dc->TextOut(center[i].x,center[i].y-height/2,str);
		SetBkMode(cdc->GetSafeHdc(), TRANSPARENT);
		if (i < num / 2)
		{
			if ( i + start_Th < GRATENUM)
			{
				{
					if (g_pMainDlg->BibanState[ i + start_Th])    cdc->SetTextColor(RGB(0, 255, 0));
					else   cdc->SetTextColor(RGB(255, 0, 0));
				}
			}
			else 
			{
				if (g_pMainDlg->BibanState[ i + start_Th-GRATENUM])    cdc->SetTextColor(RGB(0, 255, 0));
				else   cdc->SetTextColor(RGB(255, 0, 0));
			}
				/*cdc->SelectObject(pen);
			else   cdc->SelectObject(pen1);*/
			cdc->TextOut((rect_t[i].TopLeft().x + rect_t[i].BottomRight().x) / 2, (rect_t[i].TopLeft().y + rect_t[i].BottomRight().y) / 2 - height / 2, str);//�������

		}
		else {
			if (i + start_HK < GRATENUM)
			{
				if (g_pMainDlg->BibanState[start_HK + i])  cdc->SetTextColor(RGB(0, 255, 0));
				else   cdc->SetTextColor(RGB(255, 0, 0));

			}
			else
			{
				if (g_pMainDlg->BibanState[start_HK +  i - GRATENUM])    cdc->SetTextColor(RGB(0, 255, 0));
				else   cdc->SetTextColor(RGB(255, 0, 0));
			}
			
				/*cdc->SelectObject(pen);
			else   cdc->SelectObject(pen1);*/
			cdc->TextOut((rect_t[i].TopLeft().x + rect_t[i].BottomRight().x) / 2, (rect_t[i].TopLeft().y + rect_t[i].BottomRight().y) / 2 - height / 2, str);//�������
		}
		//cdc->TextOut((rect[i].TopLeft().x + rect[i].BottomRight().x) / 2, (rect[i].TopLeft().y + rect[i].BottomRight().y) / 2 - height / 2, str);//�������							
		//dc.TextOut((lefttop[i].x+rightbuttom[i].x)/2,(lefttop[i].y+rightbuttom[i].y)/2-r/4,str);//�������
		cdc->SelectObject(oldfont);
		/*cdc->SelectObject(oldpen);*/
		DeleteObject(newfont);
	}
	
	//img.release();
}
int CLocationMap::getindex()
{
	return index;
}

void CLocationMap::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	for (int i = 0; i < num; i++)
	{
		if (rect[i].PtInRect(point))
		{


			if (i < 10)
			{
				index =  i + start_Th+1;

			}
			else
			{
				index = start_HK +  i+1;
			}
			if (index > GRATENUM)
			{
				index -= GRATENUM;
			}
			pImageshow->ShowWindow(SW_SHOWNORMAL);
			break;
		}

	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}
void CLocationMap::PaintCircle(CDC * cdc, BOOL state, CRect rect)
{
	if (state)
	{
		CBrush brush(RGB(0, 255, 0));//�����Լ���Ҫ�����ɫ����ɫ
		CBrush *oldbrush;
		oldbrush = cdc->SelectObject(&brush);//ѡ�µĻ�ˢ
										   //dc.Ellipse(left,top,left+d/3,top+d/3);
		cdc->Ellipse(rect);
		cdc->SelectObject(oldbrush);//
	}
	else
	{
		CBrush brush(RGB(255, 0, 0));//�����Լ���Ҫ�����ɫ����ɫ
		CBrush *oldbrush;
		oldbrush = cdc->SelectObject(&brush);//ѡ�µĻ�ˢ
										   //dc.Ellipse(left,top,left+d/3,top+d/3);
		cdc->Ellipse(rect);
		cdc->SelectObject(oldbrush);//

	}
}
void CLocationMap::DrawlineArrow(CPoint p1, CPoint p2, double theta, int length)
{
	theta = CV_PI*theta / 180;//ת��Ϊ����
	double Px, Py, P1x, P1y, P2x, P2y;
	//��P2Ϊԭ��õ�����P2P1��P��
	Px = p1.x - p2.x;
	Py = p1.y - p2.y;
	//����P��תtheta�ǵõ�����P1
	P1x = Px*cos(theta) - Py*sin(theta);
	P1y = Px*sin(theta) + Py*cos(theta);
	//����P��ת-theta�ǵõ�����P2
	P2x = Px*cos(-theta) - Py*sin(-theta);
	P2y = Px*sin(-theta) + Py*cos(-theta);
	//�����������ƶ�����
	double x1, x2;
	x1 = sqrt(P1x*P1x + P1y*P1y);
	P1x = P1x*length / x1;
	P1y = P1y*length / x1;
	x2 = sqrt(P2x*P2x + P2y*P2y);
	P2x = P2x*length / x2;
	P2y = P2y*length / x2;
	//ƽ�Ʊ�����ֱ�ߵ�ĩ��
	P1x = P1x + p2.x;
	P1y = P1y + p2.y;
	P2x = P2x + p2.x;
	P2y = P2y + p2.y;
	CClientDC dc(this);//��ȡ�ͻ�����DC
	CPen pen, pen1, *oldpen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	pen1.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	oldpen = dc.SelectObject(&pen);
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p2.x, p2.y);
	dc.SelectObject(&pen1);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(P1x, P1y);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(P2x, P2y);
	dc.SelectObject(oldpen);
}
void CLocationMap::DrawFlashLight(CDC *dc,BOOL  SolidOrNot)
{
	//��Ӻ���������
	const int num = 6;
	CRect light[num];
	int  r = 10;
	int  dis = 20;
	//����ѡȡλ�������ε�����
	//center=((rect[9].left+rect[10].left)/2,(rect[9].top+rect[9].buttom)/2);
	//center=((rect[0].left+rect[19].left)/2,(rect[0].top+rect[0].buttom)/2);
	CPoint  center[num], lefttop[num], rightbuttom[num];
	center[1]= CPoint((rect[0].left + rect[19].right) / 2, (rect[0].top + rect[0].bottom) / 2);
	center[0] = CPoint((rect[0].left + rect[19].right) / 2-4*r- dis, (rect[0].top + rect[0].bottom) / 2);
	center[2] = CPoint((rect[0].left + rect[19].right) / 2 + 4 * r+ dis, (rect[0].top + rect[0].bottom) / 2);
	center[3] = CPoint((rect[9].left + rect[10].right) / 2 - 4 * r- dis, (rect[9].top + rect[9].bottom) / 2);
	center[4] = CPoint((rect[9].left + rect[10].right) / 2 , (rect[9].top + rect[9].bottom) / 2);
	center[5] = CPoint((rect[9].left + rect[10].right) / 2 + 4 * r+ dis,(rect[9].top + rect[9].bottom) / 2);
	for (int i = 0; i < num; i++)
	{
		lefttop[i] = CPoint(center[i].x - r, center[i].y - r);
		rightbuttom[i] = CPoint(center[i].x + r, center[i].y + r);
		light[i] = CRect(lefttop[i], rightbuttom[i]);
		

	
		if (SolidOrNot)
		{
			CBrush brush(RGB(0, 255, 0));//�����Լ���Ҫ�����ɫ����ɫ
			CBrush *oldbrush;
			oldbrush = dc->SelectObject(&brush);//ѡ�µĻ�ˢ
												//dc.Ellipse(left,top,left+d/3,top+d/3);
			dc->Ellipse(light[i]);
			dc->SelectObject(oldbrush);//

		} 
		else
		{
			dc->Ellipse(light[i]);
		}
	}
}
void CLocationMap::ShowMatToWnd(CDC *hDc,CRect drect, Mat& img)
{

	if (img.empty())
		return;
	static BITMAPINFO *bitMapinfo = NULL;
	static bool First = TRUE;
	if (First)
	{
		BYTE *bitBuffer = new BYTE[40 + 4 * 256];//����һ���ڴ�����
		if (bitBuffer == NULL)
		{
			return;
		}
		First = FALSE;
		memset(bitBuffer, 0, 40 + 4 * 256);
		bitMapinfo = (BITMAPINFO *)bitBuffer;
		bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitMapinfo->bmiHeader.biPlanes = 1;
		//for (int i = 0; i < 256; i++)
		//{ //��ɫ��ȡֵ��Χ (0-255)
		//	bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
		//}
	}

	bitMapinfo->bmiHeader.biBitCount = img.channels() * 8;
	bitMapinfo->bmiHeader.biHeight = -img.rows;
	bitMapinfo->bmiHeader.biWidth = img.cols;


	//CDC *hDc = GetDC();
	SetStretchBltMode(hDc->GetSafeHdc(), COLORONCOLOR);

	StretchDIBits(hDc->GetSafeHdc(),
		drect.left,
		drect.top,
		drect.Width(),					//��ʾ���ڿ��
		drect.Height(),					//��ʾ���ڸ߶�
		0,
		0,
		img.cols,     //ͼ����
		img.rows,     //ͼ��߶�
		img.data,
		bitMapinfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	DeleteObject(bitMapinfo);
	ReleaseDC(hDc);
}

void CLocationMap::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Event_WriteExcel.SetEvent();
}
void CLocationMap::DoGetUsersData(CString starttime, CString endtime ,int Chehao,int BiBanHao)
{
	//CDatabase m_db;
	CString sql;
	if (Chehao==0&&BiBanHao==0)
	{
		
		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' order by Time asc"), starttime, endtime);
	}
	else if (Chehao == 0 && BiBanHao> 0&&BiBanHao<=10)
	{
		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' and BiBanHao='%d'  order by Time asc"), starttime, endtime,BiBanHao);
	}
	else if (BiBanHao == 0 && Chehao > 0 && Chehao <450)
	{
		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' and Chehao='%d'  order by Time asc"), starttime, endtime, Chehao);
	}
	else if (BiBanHao > 0 && BiBanHao <= 10 && Chehao > 0 && Chehao < 450)
	{
		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' and Chehao='%d' and BiBanHao='%d' order by Time asc"), starttime, endtime, Chehao, BiBanHao);
	}
	else
	{
		AfxMessageBox(_T("��������ȷ�ĳ��ź������"));
		return;
	}

	TRY
	{
		if (!g_pMainDlg->m_Client.IsConnected())////20200928�������Խ�����ݿ�ͨѶʧ������
		{
			g_pMainDlg->m_CEvent_EventThSocket.SetEvent();
			return;
		}
		CRecordset rs2(&g_pMainDlg->m_db);
	rs2.Open(CRecordset::dynaset,sql);
	if (rs2.GetRecordCount() == 0)
	{
		AfxMessageBox(_T("û�з��������ļ�¼"));
		rs2.Close();
		return;
	}
	rs2.MoveFirst();
	int xuhao = 0;
	while (!rs2.IsEOF())
	{
		CString str_Time, str_CheHao, str_BiBanHao, str_LeiXing, str_ShuLiang, str_LuJing;
		rs2.GetFieldValue(_T("Time"), str_Time);
		rs2.GetFieldValue(_T("CheHao"), str_CheHao);
		rs2.GetFieldValue(_T("BiBanHao"), str_BiBanHao);
		rs2.GetFieldValue(_T("LeiXing"), str_LeiXing);
		rs2.GetFieldValue(_T("ShuLiang"), str_ShuLiang);
		rs2.GetFieldValue(_T("LuJing"), str_LuJing);

		int iIndex = m_CListCtrl.GetItemCount();//����ؼ���ʾ
		//LV_ITEM lvItem;
		//lvItem.mask = LVIF_TEXT;
		//lvItem.iItem = iIndex;				//����
		//lvItem.iSubItem = 0;
		//CString temp = _T("");
		//lvItem.pszText = (wchar_t*)(LPCTSTR)temp;
		//m_CListCtrl.InsertItem(&lvItem);
		xuhao++;
		CString str;
		str.Format(_T("%d"), xuhao);
		int nRow = m_CListCtrl.InsertItem(iIndex, str);
		m_CListCtrl.SetItemText(nRow, 1, str_Time);
		m_CListCtrl.SetItemText(nRow, 2, str_CheHao);
		m_CListCtrl.SetItemText(nRow, 3, str_BiBanHao);
		m_CListCtrl.SetItemText(nRow, 4, str_LeiXing);
		m_CListCtrl.SetItemText(nRow, 5, str_ShuLiang);
		m_CListCtrl.SetItemText(nRow, 6, str_LuJing);
		if (nRow > 100) //��ʾ���100������
		{
			m_CListCtrl.DeleteItem(0);
		}

		rs2.MoveNext();
	}
	rs2.Close();
	}
		CATCH(CDBException, ex)
	{
		if (ex->m_strError == _T("ͨѶ����ʧ��\n"))
		{
			//SetDlgItemText(IDC_CONNECTSQL,"ͨѶ����ʧ��");
		}
		return;
	}
	AND_CATCH(CMemoryException, pEx)
	{
		pEx->ReportError();
		AfxMessageBox(_T("memory exception"));
	}
	AND_CATCH(CException, e)
	{
		TCHAR szError[100];
		e->GetErrorMessage(szError, 100);
		AfxMessageBox(szError);
	}
	END_CATCH
}
void  CLocationMap::DODeleteData(int CheHao,int BiBanHao)
{
	TRY
	{
	g_pMainDlg->m_db.BeginTrans();
	CRecordset rs(&g_pMainDlg->m_db);
	CString sql;
	sql.Format(_T("delete from nData where CheHao = '%d'and BiBanHao = '%d'"), CheHao, BiBanHao);
	TRACE(sql);
	g_pMainDlg->m_db.ExecuteSQL(sql);
	g_pMainDlg->m_db.CommitTrans();
	}
		CATCH(CDBException, ex)
	{
		AfxMessageBox(ex->m_strError);
		AfxMessageBox(ex->m_strStateNativeOrigin);
	}
	AND_CATCH(CMemoryException, pEx)
	{
		pEx->ReportError();
		AfxMessageBox(_T("memory exception"));
	}
	AND_CATCH(CException, e)
	{
		TCHAR szError[100];
		e->GetErrorMessage(szError, 100);
		AfxMessageBox(szError);
	}
	END_CATCH
}

//void CLocationMap::DoUpdateUsersData(CString starttime, CString endtime, int Chehao, int BiBanHao)
//{
//	//CDatabase m_db;
//	CString sql;
//	if (Chehao == 0 && BiBanHao == 0)
//	{
//
//		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' order by Time asc"), starttime, endtime);
//	}
//	else if (Chehao == 0 && BiBanHao > 0 && BiBanHao <= 10)
//	{
//		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' and BiBanHao='%d'  order by Time asc"), starttime, endtime, BiBanHao);
//	}
//	else if (BiBanHao == 0 && Chehao > 0 && Chehao < 450)
//	{
//		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' and Chehao='%d'  order by Time asc"), starttime, endtime, Chehao);
//	}
//	else if (BiBanHao > 0 && BiBanHao <= 10 && Chehao > 0 && Chehao < 450)
//	{
//		sql.Format(_T("select * from nData where Time > '%s' and Time  < '%s' and Chehao='%d' and BiBanHao='%d' order by Time asc"), starttime, endtime, Chehao, BiBanHao);
//	}
//	else
//	{
//		AfxMessageBox(_T("��������ȷ�ĳ��ź������"));
//		return;
//	}
//
//	TRY
//	{
//		CRecordset rs2(&g_pMainDlg->m_db);
//	rs2.Open(CRecordset::dynaset,sql);
//	if (rs2.GetRecordCount() == 0)
//	{
//		AfxMessageBox(_T("û�з��������ļ�¼"));
//		rs2.Close();
//		return;
//	}
//	rs2.MoveFirst();
//	int xuhao = 0;
//	while (!rs2.IsEOF())
//	{
//		CString str_Time, str_CheHao, str_BiBanHao, str_LeiXing, str_ShuLiang, str_LuJing;
//		rs2.GetFieldValue(_T("Time"), str_Time);
//		rs2.GetFieldValue(_T("CheHao"), str_CheHao);
//		rs2.GetFieldValue(_T("BiBanHao"), str_BiBanHao);
//		rs2.GetFieldValue(_T("LeiXing"), str_LeiXing);
//		rs2.GetFieldValue(_T("ShuLiang"), str_ShuLiang);
//		rs2.GetFieldValue(_T("LuJing"), str_LuJing);
//
//		int iIndex = m_CListCtrl.GetItemCount();//����ؼ���ʾ
//												//LV_ITEM lvItem;
//												//lvItem.mask = LVIF_TEXT;
//												//lvItem.iItem = iIndex;				//����
//												//lvItem.iSubItem = 0;
//												//CString temp = _T("");
//												//lvItem.pszText = (wchar_t*)(LPCTSTR)temp;
//												//m_CListCtrl.InsertItem(&lvItem);
//		xuhao++;
//		CString str;
//		str.Format(_T("%d"), xuhao);
//		int nRow = m_CListCtrl.InsertItem(iIndex, str);
//		m_CListCtrl.SetItemText(nRow, 1, str_Time);
//		m_CListCtrl.SetItemText(nRow, 2, str_CheHao);
//		m_CListCtrl.SetItemText(nRow, 3, str_BiBanHao);
//		m_CListCtrl.SetItemText(nRow, 4, str_LeiXing);
//		m_CListCtrl.SetItemText(nRow, 5, str_ShuLiang);
//		if (nRow > 100) //��ʾ���100������
//		{
//			m_CListCtrl.DeleteItem(0);
//		}
//
//		rs2.MoveNext();
//	}
//	rs2.Close();
//	}
//		CATCH(CDBException, ex)
//	{
//		if (ex->m_strError == _T("ͨѶ����ʧ��\n"))
//		{
//			//SetDlgItemText(IDC_CONNECTSQL,"ͨѶ����ʧ��");
//		}
//		return;
//	}
//	AND_CATCH(CMemoryException, pEx)
//	{
//		pEx->ReportError();
//		AfxMessageBox(_T("memory exception"));
//	}
//	AND_CATCH(CException, e)
//	{
//		TCHAR szError[100];
//		e->GetErrorMessage(szError, 100);
//		AfxMessageBox(szError);
//	}
//	END_CATCH
//}
void CLocationMap::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Event_QueryDB.SetEvent();
}
void CLocationMap::InitMsgList(BOOL isInitFir, CListCtrl  &m_List_Msg)
{
	if (isInitFir)
	{
		DWORD dwStyle = m_List_Msg.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;	//ʹ���и���
		dwStyle |= LVS_EX_GRIDLINES;		//������
		m_List_Msg.SetExtendedStyle(dwStyle); // ����ѡ��������
		CRect rt;
		m_List_Msg.GetWindowRect(rt);

		int fircolwidth = 60;
		int colsNum = 6 - 1;
		int w = (rt.Width() - fircolwidth) / colsNum;
		m_List_Msg.InsertColumn(0, _T("���"), LVCFMT_CENTER, fircolwidth);
		m_List_Msg.InsertColumn(1, _T("ʱ��"), LVCFMT_CENTER, w);
		m_List_Msg.InsertColumn(2, _T("���"), LVCFMT_CENTER, w);
		m_List_Msg.InsertColumn(3, _T("�����"), LVCFMT_CENTER, w);
		m_List_Msg.InsertColumn(4, _T("����"), LVCFMT_CENTER, w);
		m_List_Msg.InsertColumn(5, _T("����"), LVCFMT_CENTER, w);
		m_List_Msg.InsertColumn(6, _T("·��"), LVCFMT_CENTER, 0);
	
	}
	CRect rt;
	m_List_Msg.GetWindowRect(rt);

	int fircolwidth = 60;
	int colsNum = 6 - 1;
	int w = (rt.Width() - fircolwidth) / colsNum;
	m_List_Msg.SetColumnWidth(0, fircolwidth);
	for (int i = 1; i < 6; i++)
	{
		m_List_Msg.SetColumnWidth(i, w);
	}
}


void CLocationMap::OnEnChangeEditChehao()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString str;
	GetDlgItemText(IDC_EDIT_CHEHAO, str);
	m_CheHao = _ttoi(str);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CLocationMap::OnEnChangeEditBibanhao()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString str;
	GetDlgItemText(IDC_EDIT_BIBANHAO, str);
	m_BiBanHao = _ttoi(str);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CLocationMap::OnBnClickedButtonClearwarn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Login!=TRUE)
	{
		MessageBox(_T("���¼���ٽ��д˲�����"), _T("��ʾ"));
		return;
	}
	else
	{ 
		POSITION pos = m_CListCtrl.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			return;
		}

		DelReplyWithPosition(pos);
	}
	
	

}
void CLocationMap::DelReplyWithPosition(POSITION pos)
{
	int row, col;
	row = _ttoi(m_CListCtrl.GetItemText((int)pos - 1, 2));
	col = _ttoi(m_CListCtrl.GetItemText((int)pos - 1, 3));
	g_pMainDlg->SetGrateStateNormal(row - 1, col - 1);
	DODeleteData(row,col);
	int iItemIndex = m_CListCtrl.GetNextSelectedItem(pos);
	if (pos != NULL)
	{
		DelReplyWithPosition(pos);
		
		m_CListCtrl.DeleteItem(iItemIndex);
		if (g_pMainDlg->m_List_Msg.GetItemCount()>=iItemIndex&&!g_pMainDlg->m_CStringA_msgTmgPath.IsEmpty()&&g_pMainDlg->m_CStringA_msgTmgPath.GetCount()>=iItemIndex)
		{
			g_pMainDlg->m_List_Msg.DeleteItem(iItemIndex);
			g_pMainDlg->m_CStringA_msgTmgPath.RemoveAt(iItemIndex);
		}
		
		
	}
	else
	{
		m_CListCtrl.DeleteItem(iItemIndex);
		if (g_pMainDlg->m_List_Msg.GetItemCount() >= iItemIndex&&!g_pMainDlg->m_CStringA_msgTmgPath.IsEmpty()&&g_pMainDlg->m_CStringA_msgTmgPath.GetCount()>=iItemIndex)
		{
			g_pMainDlg->m_List_Msg.DeleteItem(iItemIndex);
			g_pMainDlg->m_CStringA_msgTmgPath.RemoveAt(iItemIndex);
		}
		
	}
}



void CLocationMap::OnNMDblclkListMsg2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_CListCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		/*g_pMainDlg->m_int_msgSlt = m_CListCtrl.GetNextSelectedItem(pos);
		if (g_pMainDlg->m_int_msgSlt >= 0)
		{
			g_pMainDlg->pMsgImg->UpdateImg();
			g_pMainDlg->pMsgImg->ShowWindow(SW_SHOWNORMAL);
		}*/
		int nitem=m_CListCtrl.GetNextSelectedItem(pos);
		if (nitem>=0)
		{
			if (m_CListCtrl.GetItemText(nitem, 6).IsEmpty()) //if (m_CListCtrl.GetItemText(nitem, 6).GetLength() == 0)
			{
				g_pMainDlg->m_int_msgSlt = nitem;
				if (g_pMainDlg->m_int_msgSlt >= 0)
				{
					g_pMainDlg->pMsgImg->UpdateImg();
					g_pMainDlg->pMsgImg->ShowWindow(SW_SHOWNORMAL);
				}
			}
			else
			{
				string  strImg = CString2String(m_CListCtrl.GetItemText(nitem, 6));
				Mat m_Mat_Img = cv::imread(strImg, IMREAD_COLOR);
				m_Mat_Img.copyTo(g_pMainDlg->pMsgImg->m_Mat_Img);
				g_pMainDlg->pMsgImg->UpdateImg1();
				g_pMainDlg->pMsgImg->ShowWindow(SW_SHOWNORMAL);
			}
			/*int cout = m_CListCtrl.GetHeaderCtrl()->GetItemCount();
			for (int i=0;i<cout;i++)
			{
				m_CListCtrl.SetSubItemColor(nitem, i, RGB(255, 0, 0), RGB(255, 255, 255));
			}*/
			m_CListCtrl.SetItemData(nitem, COLOR_GRAY);
			
			//m_CListCtrl.SetSubItemColor(nitem, i, RGB(255, 0, 0), RGB(255, 255, 255));
			
		}
	}
	

	*pResult = 0;
}
std::string CLocationMap::CString2String(CString cs)
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

void CLocationMap::OnNMCustomdrawListMsg2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	{
		if (COLOR_BLUE == lplvcd->nmcd.lItemlParam)
		{
			lplvcd->clrTextBk = RGB(255, 255, 255);
			lplvcd->clrText = RGB(51, 153, 255);
		}
		else if (COLOR_RED == lplvcd->nmcd.lItemlParam)
		{
			lplvcd->clrTextBk = RGB(255, 255, 255); 		//������ɫ
			lplvcd->clrText = RGB(255, 0, 0);	//������ɫ
		}
		else if (COLOR_DEFAULT == lplvcd->nmcd.lItemlParam)
		{
			lplvcd->clrTextBk = RGB(255, 255, 255);
			lplvcd->clrText = RGB(0, 0, 0);
		}
		else if (COLOR_YELLOW == lplvcd->nmcd.lItemlParam)
		{
			lplvcd->clrTextBk = RGB(255, 255, 255);
			lplvcd->clrText = RGB(255, 255, 0);
		}
		else if (COLOR_ORANGE == lplvcd->nmcd.lItemlParam)
		{
			lplvcd->clrTextBk = RGB(255, 255, 255);
			lplvcd->clrText = RGB(255, 97, 0);
		}
		else if (COLOR_GRAY == lplvcd->nmcd.lItemlParam)
		{
			lplvcd->clrTextBk = RGB(255, 255, 255);
			lplvcd->clrText = RGB(192, 192, 192);
		}
		break;
	}
	default:
	{
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//�����У���Ȼ��û��Ч��
	*pResult |= CDRF_NOTIFYITEMDRAW;		//�����У���Ȼ��û��Ч��
	return;
}


void CLocationMap::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	if (GetDlgItemText(IDC_BUTTON_LOGIN, str), str == _T("��¼"))
	{
		pPassWordDlg->ShowWindow(SW_SHOWNORMAL);
	}
	else if (GetDlgItemText(IDC_BUTTON_LOGIN, str), str == _T("ע��"))
	{
		m_Login = FALSE;
		SetDlgItemText(IDC_BUTTON_LOGIN, _T("��¼"));
	}
	
	
}
void CLocationMap::ChangeLoginState()
{
	if (m_Login!=FALSE)
	{
		m_Login = FALSE;
	}
	else
	{
		m_Login = TRUE;
	}
}

//BOOL CLocationMap::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	//return true;
//	return CDialogEx::OnEraseBkgnd(pDC);
//}


//BOOL CLocationMap::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	/*CBrush br;
//	br.CreateDIBPatternBrush()*/
//	
//	//CBitmap   bmpBackground;
//	//bmpBackground.LoadBitmap(IDB_BITMAP1);
//	////IDB_BITMAP1���Լ���ͼ��Ӧ��ID 
//	//BITMAP   bitmap;
//	//bmpBackground.GetBitmap(&bitmap);
//	//
//	//dc.StretchBlt(0, 0, rect1.Width(), rect1.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
//	CRect rect;
//
//	CDC   memDC;
//
//	CBitmap bmpBackground;
//
//	CBitmap* pOldMemBmp = NULL;
//
//	GetWindowRect(&rect);
//
//	bmpBackground.LoadBitmap(IDB_BITMAP1);
//
//	memDC.CreateCompatibleDC(pDC);
//
//	pOldMemBmp = memDC.SelectObject(&bmpBackground);
//
//	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
//	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bmpBackground.bmWidth, bmpBackground.bmHeight, SRCCOPY);
//	if (pOldMemBmp)
//
//		memDC.SelectObject(pOldMemBmp);
//
//	return   TRUE;
//	//return CDialogEx::OnEraseBkgnd(pDC);
//}


HBRUSH CLocationMap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (GetDlgItem(IDC_STATIC_LOST)->m_hWnd == pWnd->m_hWnd)
	{
		//pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont  m_Font;
		m_Font.CreatePointFont(300, _T("����"));
		pDC->SelectObject(&m_Font);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	//pDC->SetBkColor(RGB(255, 255, 200));
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//hbr = m_Brush;
	if (GetDlgItem(IDC_STATIC_CHEHAO)->m_hWnd == pWnd->m_hWnd)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont  m_Font;
		m_Font.CreatePointFont(120, _T("����"));
		pDC->SelectObject(&m_Font);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	if (GetDlgItem(IDC_STATIC_BIBANHAO)->m_hWnd == pWnd->m_hWnd)
	{
		//pDC->SetBkColor(RGB(0, 0, 0));
		//CFont  m_Font;
		//m_Font.CreatePointFont(10, _T("����"));
		//pDC->SelectObject(&m_Font);
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont  m_Font;
		m_Font.CreatePointFont(120, _T("����"));
		pDC->SelectObject(&m_Font);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	if (GetDlgItem(IDC_STATIC_THSTATE)->m_hWnd == pWnd->m_hWnd)
	{
		//pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont  m_Font;
		m_Font.CreatePointFont(200, _T("����"));
		pDC->SelectObject(&m_Font);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}
unsigned int CLocationMap::ThreadProc_WriteExcel(LPVOID lpParam)
{
	CLocationMap* pDlg = (CLocationMap*)lpParam;
	COleDateTime pretime = COleDateTime::GetCurrentTime();
	COleDateTime nowtime = COleDateTime::GetCurrentTime();
	while (1)
	{
		
		WaitForSingleObject(pDlg->m_Event_WriteExcel, INFINITE);
		nowtime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan tspan = nowtime - pretime;
		double sec = tspan.GetTotalSeconds();
		if (sec < 2)
		{
			continue;
		}
		pDlg->WriteExcel();
	}
}
unsigned int CLocationMap::ThreadProc_QueryDB(LPVOID lpParam)
{
	CLocationMap* pDlg = (CLocationMap*)lpParam;
	while (1)
	{

		WaitForSingleObject(pDlg->m_Event_QueryDB, INFINITE);
		pDlg->QueryDB();
	}
}
void CLocationMap::WriteExcel()
{
	//ExportAsExcel(NULL, m_CListCtrl,NULL);
	CApplication app1;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	CRange iCell;
	COleVariant vResult;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	//CString Result, sPath;
	CString Result = _T("");
	//TCHAR sFileName[256];
	//GetModuleFileName(AfxGetInstanceHandle(), sFileName, 255);  // ȡ�õ�ǰ��ִ���ļ���·����������sFileName��  
	//sPath = sFileName;
	//int index = sPath.ReverseFind('\\');                        // �������ָ���ַ�  
	//sPath = sPath.Left(index);
	CTime nowtime = CTime::GetCurrentTime();
	CString tmpstr = nowtime.Format("%Y%m%d%H%M%S");
	Result = theApp.str_EXEpath + _T("\\") + tmpstr + _T(".xls");
	if (S_OK != CoInitialize(NULL))
	{
		//AfxMessageBox(_T("Initialize com failed..."));
		//return ;
	}
	if (!app1.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("�޷�����Excel!"));
		return;
	}

	app1.put_Visible(FALSE);
	books.AttachDispatch(app1.get_Workbooks());
	book.AttachDispatch(books.Add(covOptional));
	sheets.AttachDispatch(book.get_Worksheets()); //�õ�Worksheets 
	sheet.AttachDispatch(sheets.get_Item(_variant_t("sheet1"))); //�õ�sheet1 
	range.AttachDispatch(sheet.get_Cells());

	range.put_NumberFormat(COleVariant(_T("@")));//���ó��ı���ʽ
	range.put_HorizontalAlignment(_variant_t((long)-4108));
	range = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1"))); //��ȡ��Ԫ������
	range.put_ColumnWidth(_variant_t((long)10));  //�����п�
												  /*range.put_Item(COleVariant(long(1)), COleVariant(long(1)), COleVariant(_T("ʱ��")));
												  range.put_Item(COleVariant(long(1)), COleVariant(long(2)), COleVariant(_T("����¶�1")));
												  range.put_Item(COleVariant(long(1)), COleVariant(long(3)), COleVariant(_T("����¶�2")));
												  range.put_Item(COleVariant(long(1)), COleVariant(long(4)), COleVariant(_T("����¶�3")));
												  range.put_Item(COleVariant(long(1)), COleVariant(long(5)), COleVariant(_T("����¶�4")));
												  range.put_Item(COleVariant(long(1)), COleVariant(long(6)), COleVariant(_T("����¶�5")));*/
	CRange pRange;
	pRange = sheet.get_Range(COleVariant(_T("A451")), COleVariant(_T("E451")));
	pRange.Merge(COleVariant((long)0));
	CRange pRange2;
	pRange2 = sheet.get_Range(COleVariant(_T("F451")), COleVariant(_T("K451")));
	pRange2.Merge(COleVariant((long)0));
	for (int i = 0; i<10; i++)
	{
		CString str;
		str.Format(_T("����%d"), i + 1);
		range.put_Item(COleVariant(long(1)), COleVariant(long(i + 2)), COleVariant(str));
	}
	for (int i = 0; i <GRATENUM; i++)
	{
		CString str;
		str.Format(_T("���%d"), i + 1);
		range.put_Item(COleVariant(long(i + 2)), COleVariant(long(1)), COleVariant(str));
	}
	int  nCount = m_CListCtrl.GetItemCount();
	CString str;
	int  LostNum = 0;
	for (int i = 0; i <GRATENUM; i++)
	{

		for (int j = 0; j <10; j++)
		{
			switch (g_pMainDlg->m_int_GrateState[i][j])
			{
			case GRATE_Normal:
				str = _T(" ");
				break;
			case GRATE_NotReset:
				str = _T("δ��λ");
				break;
			case GRATE_Lost:
				str = _T("ȱʧ");
				LostNum++;
				break;
			case GRATE_LostandNotReset:
				str = _T("ȱʧ+δ��λ");
				LostNum++;
				break;
			default:
				break;
			}
			range.put_Item(COleVariant(long(i + 2)), COleVariant(long(j + 2)), COleVariant(str));
		}
	}
	str.Format(_T("����ȱʧ����    %d"), LostNum);
	pRange.put_Formula(COleVariant(str));
	str.Format(_T("����ȱʧ�ʣ�    %f%%"), LostNum*10.0 / (GRATENUM));
	pRange2.put_Formula(COleVariant(str));
	book.SaveAs(COleVariant(Result.GetBuffer(100)), covOptional, covOptional, covOptional, covOptional,
		covOptional, 0, covOptional, covOptional, covOptional, covOptional, covOptional);
	///�����Զ���,δ�ܳɹ�
	/*LPDISPATCH lpDisp = NULL;

	lpDisp=books.Open(Result.GetBuffer(100), vtMissing, vtMissing,
	vtMissing, vtMissing, vtMissing, vtMissing,
	vtMissing, vtMissing, vtMissing, vtMissing,
	vtMissing, vtMissing, vtMissing, vtMissing);
	book.AttachDispatch(lpDisp);*/


	books.Close();
	app1.Quit();

	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app1.ReleaseDispatch();


	CString msb;
	msb.Format(_T("�����ɹ����ļ�������%s"), Result);
	AfxMessageBox(msb);
}
void CLocationMap::QueryDB()
{
	m_CListCtrl.DeleteAllItems();
	CTime tempTime1;
	CTime tempTime2;
	CString sTime, eTime;
	//ȡ�ÿ�ʼʱ��
	CDateTimeCtrl * pWndTemp1 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER3);
	CDateTimeCtrl * pWndTemp2 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER4);
	pWndTemp1->GetTime(tempTime1);
	pWndTemp2->GetTime(tempTime2);
	sTime = tempTime1.Format("%Y-%m-%d") + tempTime2.Format(" %H_%M_%S");
	//ȡ�ý���ʱ��
	pWndTemp1 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER5);
	pWndTemp2 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER6);
	pWndTemp1->GetTime(tempTime1);
	pWndTemp2->GetTime(tempTime2);
	eTime = tempTime1.Format("%Y-%m-%d") + tempTime2.Format(" %H_%M_%S");

	DoGetUsersData(sTime, eTime, m_CheHao, m_BiBanHao);
}