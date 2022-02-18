#include "stdafx.h"
#include "ImageUtility.h"


ImageUtility::ImageUtility(void)
{
}


ImageUtility::~ImageUtility(void)
{
}


// ʵ��cv::Mat �ṹ�� CImage�ṹ��ת��
void ImageUtility::MatToCImage(Mat& mat, CImage& cImage)
{
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	cImage.Destroy();//��һ���Ƿ�ֹ�ظ���������ڴ�����
	cImage.Create(width,height,8*channels);

	uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //��ȡCImage�����ش�������ָ��
	int step = cImage.GetPitch();//ÿ�е��ֽ���,ע���������ֵ�����и�
	
	// �����1��ͨ����ͼ��(�Ҷ�ͼ��) DIB��ʽ����Ҫ�Ե�ɫ������  
	// CImage�������˵�ɫ�壬����Ҫ�������и�ֵ��
	if (1 == channels)
	{
		RGBQUAD* ColorTable;  
		int MaxColors=256;  
		//�������ͨ��CI.GetMaxColorTableEntries()�õ���С(�������CI.Load����ͼ��Ļ�)  
		ColorTable = new RGBQUAD[MaxColors];  
		cImage.GetColorTable(0,MaxColors,ColorTable);//������ȡ��ָ��  
		for (int i=0; i<MaxColors; i++)  
		{  
			ColorTable[i].rgbBlue = (BYTE)i;  
			//BYTE��ucharһ���£���MFC�ж�����  
			ColorTable[i].rgbGreen = (BYTE)i;  
			ColorTable[i].rgbRed = (BYTE)i;  
		}  
		cImage.SetColorTable(0,MaxColors,ColorTable);  
		delete []ColorTable;  
	}
	

	for (int i = 0; i < height; i++)
	{
		ps = mat.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if (1 == channels)
			{
				*(pimg + i*step + j) = ps[j];
			}
			else if (3 == channels)
			{
				*(pimg + i*step + j*3) = ps[j*3];
				*(pimg + i*step + j*3 + 1) = ps[j*3 + 1];
				*(pimg + i*step + j*3 + 2) = ps[j*3 + 2];
			}
		}
	}
}

void ImageUtility::ResizeMat(cv::Mat &img, int width, int height)
{

	//ͼ��Ŀ�͸�
	int h = img.size().height;
	int w = img.size().width;
	float scale,scaleh,scalew;
	
	//�������������,�ĸ����ŵı������ʹ���ĸ����Է������ؼ���Χ
	scaleh = (float)(float(h)/float(height)); //����Ӷ���float����Ȼ�ʹ���
	scalew = (float)(float(w)/float(width));
	scale = (scaleh>scalew)?scaleh:scalew;
	
	//�����ͼƬ���µĿ�͸�
	int  nw = (int )(w/scale);
	int nh = (int)(h/scale);

	Size size = Size(nw,nh);
	cv::resize( img,img,size);

}

void ImageUtility::DisplayImage(cv::Mat imgIn, UINT ID,HWND hDlg)
{
	//�ڶԻ�����ʹ��GetDlgItemҪת��һ��
	CImage cimg;
	Mat img;
	imgIn.copyTo(img);
	CDC* pDC = CWnd::FromHandle(GetDlgItem(hDlg, ID))->GetDC();
	HDC hDC = pDC ->GetSafeHdc();

	CRect rect;
	CWnd::FromHandle(GetDlgItem(hDlg, ID)) ->GetClientRect(&rect);

	//�ȶԿؼ��ϵ�ͼƬ�������
	//pDC->FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), RGB(240,240,240));
    
	//��ͼ���������,���տؼ��Ŀ�͸�
	int picCtrlW = rect.right-rect.left;
	int  picCtrlH = rect.bottom - rect.top;

	ResizeMat(img, picCtrlW, picCtrlH);

	int imgW = img.cols;
	int imgH = img.rows;
	//��ͼ����ʾ�ڿؼ�������
	int drawX = (int)(picCtrlW-imgW)/2;
	int drawY = (int)(picCtrlH-imgH)/2;

	MatToCImage(img, cimg);
	cimg.Draw(hDC,rect);	
	ReleaseDC(hDlg, hDC);

}