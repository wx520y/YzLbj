// ImageShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YzLbj.h"
#include "ImageShow.h"
#include "afxdialogex.h"
#include "LocationMap.h"
#include "string"
#include "iostream"
#include "YzLbj.h"
// CImageShow �Ի���
using namespace std;
using namespace cv;
extern CLocationMap *pLocationMap;
IMPLEMENT_DYNAMIC(CImageShow, CDialogEx)
#define _IP_MARK "."
CImageShow::CImageShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_IMAGESHOW, pParent)
{
	
}

CImageShow::~CImageShow()
{
}

void CImageShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageShow, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

std::string CImageShow::CString2String(CString cs)
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
CString CImageShow::INTtoIP(uint32_t num)
{

	CString strRet = _T("");
	for (int i = 0; i < 4; i++)
	{
		//uint32_t tmp = (num >> ((3 - i) * 8)) & 0xFF;
		uint32_t tmp = (num >> ((i) * 8)) & 0xFF;
		char chBuf[8] = "";
		_itoa_s(tmp, chBuf, 10);
		strRet += chBuf;

		if (i < 3)
		{
			strRet += _IP_MARK;
		}
	}

	return strRet;
}
//// CImageShow ��Ϣ�������
//std::string toString(CString cs)
//{
//#ifdef UNICODE
//
//	//�����unicode����
//	USES_CONVERSION;
//	std::string str(W2A(cs));
//	return str;
//#else
//	//����Ƕ��ֽڹ��� 
//	std::string str(cs.GetBuffer());
//	cs.ReleaseBuffer();
//	return str;
//
//#endif // _UNICODE 
//}
//
//CString toCString(std::string str)
//{
//#ifdef UNICODE
//	//�����unicode����
//	USES_CONVERSION; CString s(str.c_str());
//	CString ans(str.c_str());
//	return ans;
//#else
//	//����Ƕ��ֽڹ��� 
//	//string ת CString
//	CString ans;
//	ans.Format("%s", str.c_str());
//	return ans;
//#endif // _UNICODE  
//}

void CImageShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CString str;
	int i = pLocationMap->getindex();
	str.Format(_T("��%d�������쳣��ʾ"), i);
	GetDlgItem(IDC_STATIC)->SetWindowText(str);
	//SetDlgItemText(IDC_STATIC,str);
	CFont cfont;
	cfont.CreatePointFont(250, _T("����"), NULL);
	GetDlgItem(IDC_STATIC)->SetFont(&cfont);
	CImageShow  *pImageShow;
	pImageShow = this;
	//string HKdir = "C:\\Users\\87482\\Desktop\\������ͼ��\\HK";
	string HKdir = CString2String(theApp.str_HKImgPath);
	//string THdir = "C:\\Users\\87482\\Desktop\\������ͼ��\\TH";
	string THdir = CString2String(theApp.str_THImgPath);
	if (!PathIsDirectoryA(HKdir.c_str())&& (!PathIsDirectoryA(THdir.c_str())))
	{
		AfxMessageBox(_T("�ļ�������"));

	}

	if (PathIsDirectoryA(HKdir.c_str()))
	{


		std::vector<std::string> path = findFile(HKdir);
		vector<int> HKnumber;
		HKnumber = HKSeekChehao(path, i);
		if (HKnumber.empty())
		{
			CString str;
			str.Format(_T("��%d��û�г�������ȱʧ����"), i);
			MessageBox(str, _T("��ʾ"));
		}
		for (unsigned int i = 0; i < HKnumber.size(); i++)
		{
			for (int j = 0; j < HKCAMNUM; j++)
			{

				string Hkip = CString2String(theApp.Hk_str_CamIp[j]);

				unsigned int no = HKnumber[i];
				if (HKCompareIP(path[no], Hkip))
				{
					string tmppath = HKdir + "\\" + path[no];
					Mat img = imread(tmppath);
					CWnd *pWnd = pImageShow->GetDlgItem(IDC_HKPIC0 + j);
					pImageShow->ShowMatToWnd(pWnd, img);
				}
			 }
		}

	}
	
	if (PathIsDirectoryA(THdir.c_str()))
	{


		std::vector<std::string> path = findFile(THdir);
		vector<int> THnumber;
		THnumber = THSeekChehao(path, i);
		if (THnumber.empty())
		{
			//CString str;
			//str.Format(_T("��%d��û�г�������δ��λ����"), i);
			//MessageBox(str, _T("��ʾ"));
			return;
		}
		for (unsigned int i = 0; i < THnumber.size(); i++)
		{
			for (int j = 0; j <THCAMNUM; j++)
			{

				string Thip = CString2String(INTtoIP(theApp.Th_int_TerminalIp[j]));

				unsigned int no = THnumber[i];
				if (THCompareIP(path[no], Thip))
				{
					string tmppath = THdir + "\\" + path[no];
					Mat img = imread(tmppath);
					CWnd *pWnd = pImageShow->GetDlgItem(IDC_THPIC0 + j);
					pImageShow->ShowMatToWnd(pWnd, img);
				}
			}
		}

	}	

				



	
}

BOOL CImageShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	/*CImageShow  *pImageShow;
	pImageShow = this;
	CWnd *pWnd = pImageShow->GetDlgItem(IDC_HKPIC0);
	string path = "C:\\Users\\87482\\Desktop\\hk.png";
	Mat img = imread(path);
	pImageShow->ShowMatToWnd(pWnd, img);*/
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CImageShow::ShowMatToWnd(CWnd *pWnd, cv::Mat& img)
{
	//WriteLog(__FUNCTION__,"#start#");
	if (img.empty())
		return FALSE;
	static BITMAPINFO *bitMapinfo = NULL;
	static bool First = TRUE;
	if (First)
	{
		BYTE *bitBuffer = new BYTE[40 + 4 * 256];//����һ���ڴ�����
		if (bitBuffer == NULL)
		{
			return FALSE;
		}
		First = FALSE;
		memset(bitBuffer, 0, 40 + 4 * 256);
		bitMapinfo = (BITMAPINFO *)bitBuffer;
		bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitMapinfo->bmiHeader.biPlanes = 1;
		for (int i = 0; i < 256; i++)
		{ //��ɫ��ȡֵ��Χ (0-255)
			bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
		}
		//delete[]bitBuffer;
	}

	bitMapinfo->bmiHeader.biBitCount = img.channels() * 8;
	bitMapinfo->bmiHeader.biHeight = -img.rows;
	bitMapinfo->bmiHeader.biWidth = img.cols;

	CRect drect;
	pWnd->GetClientRect(drect);
	CClientDC dc(pWnd);
	HDC hDc = dc.GetSafeHdc();
	SetStretchBltMode(hDc, COLORONCOLOR);

	StretchDIBits(hDc,
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
	::ReleaseDC((HWND)pWnd, hDc);
	::ReleaseDC((HWND)pWnd, dc);
	return TRUE;
}
vector<string> CImageShow::findFile(string dir)
{
	WIN32_FIND_DATAA stFD;							//����ļ���Ϣ�Ľṹ��
	HANDLE h;
	string temp;
	vector<string> ret;
	
	temp = dir + "\\*";
	h = FindFirstFileA(temp.c_str(), &stFD);			//����Ŀ¼���

	while (FindNextFileA(h, &stFD))						//��ȡĿ¼�����ӦĿ¼���������ļ�
	{
		temp = dir + "\\" + stFD.cFileName;

		if (temp == dir + "\\..")										//��һ��·��
		{
			continue;
		}
		else if (PathIsDirectoryA(temp.c_str()))		//������Ŀ¼
		{
			findFile(temp);												//�ݹ����
		}
		else
		{
			//cout << stFD.cFileName << endl;				//��ӡ�ļ���
			 ret.push_back(stFD.cFileName); 
			

		}
	}
	return ret;
}
std::vector<int> CImageShow::HKSeekChehao(std::vector<std::string> path, int chehao)
{


	 vector<int> number;
	for ( int i = 0; i <(int) path.size(); i++)
	{
		try
		{
			if (stoi(path[i].substr(13, 3)) == chehao)
			{
				number.push_back(i);
			}
		}
		catch (std::invalid_argument&)
		{
			continue;
		}
		catch (...)
		{
			continue;
		}
		
	}
	return  number;
}
std::vector<int> CImageShow::THSeekChehao(std::vector<std::string> path, int chehao)
{


	vector<int> number;
	for (int i = 0; i < (int)path.size(); i++)
	{
		try
		{
			if (stoi(path[i].substr(14, 3)) == chehao)
			{
				number.push_back(i);
			}
		}
		catch (std::invalid_argument&)
		{
			continue;
		}
		catch (...)
		{
			continue;
		}

	}
	return  number;
}
BOOL CImageShow::HKCompareIP(std::string path, std::string ip)
{
		if (path.substr(0, 12)== ip)
		{
			
			return  TRUE;
		}
	return FALSE;
}
BOOL CImageShow::THCompareIP(std::string path, std::string ip)
{
	if (path.substr(0, 13) == ip)
	{

		return  TRUE;
	}
	return FALSE;
}
CString CImageShow::ReturnPath()
{
	CString    sPath = _T("");
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return    sPath;
}
