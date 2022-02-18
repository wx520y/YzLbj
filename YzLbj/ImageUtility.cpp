#include "stdafx.h"
#include "ImageUtility.h"


ImageUtility::ImageUtility(void)
{
}


ImageUtility::~ImageUtility(void)
{
}


// 实现cv::Mat 结构到 CImage结构的转化
void ImageUtility::MatToCImage(Mat& mat, CImage& cImage)
{
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	cImage.Destroy();//这一步是防止重复利用造成内存问题
	cImage.Create(width,height,8*channels);

	uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //获取CImage的像素存贮区的指针
	int step = cImage.GetPitch();//每行的字节数,注意这个返回值有正有负
	
	// 如果是1个通道的图像(灰度图像) DIB格式才需要对调色板设置  
	// CImage中内置了调色板，我们要对他进行赋值：
	if (1 == channels)
	{
		RGBQUAD* ColorTable;  
		int MaxColors=256;  
		//这里可以通过CI.GetMaxColorTableEntries()得到大小(如果你是CI.Load读入图像的话)  
		ColorTable = new RGBQUAD[MaxColors];  
		cImage.GetColorTable(0,MaxColors,ColorTable);//这里是取得指针  
		for (int i=0; i<MaxColors; i++)  
		{  
			ColorTable[i].rgbBlue = (BYTE)i;  
			//BYTE和uchar一回事，但MFC中都用它  
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

	//图像的宽和高
	int h = img.size().height;
	int w = img.size().width;
	float scale,scaleh,scalew;
	
	//计算出缩放因子,哪个缩放的倍数大就使用哪个，以防超出控件范围
	scaleh = (float)(float(h)/float(height)); //必须加都加float，不然就错误
	scalew = (float)(float(w)/float(width));
	scale = (scaleh>scalew)?scaleh:scalew;
	
	//计算出图片的新的宽和高
	int  nw = (int )(w/scale);
	int nh = (int)(h/scale);

	Size size = Size(nw,nh);
	cv::resize( img,img,size);

}

void ImageUtility::DisplayImage(cv::Mat imgIn, UINT ID,HWND hDlg)
{
	//在对话框外使用GetDlgItem要转换一下
	CImage cimg;
	Mat img;
	imgIn.copyTo(img);
	CDC* pDC = CWnd::FromHandle(GetDlgItem(hDlg, ID))->GetDC();
	HDC hDC = pDC ->GetSafeHdc();

	CRect rect;
	CWnd::FromHandle(GetDlgItem(hDlg, ID)) ->GetClientRect(&rect);

	//先对控件上的图片进行清空
	//pDC->FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), RGB(240,240,240));
    
	//对图像进行缩放,按照控件的宽和高
	int picCtrlW = rect.right-rect.left;
	int  picCtrlH = rect.bottom - rect.top;

	ResizeMat(img, picCtrlW, picCtrlH);

	int imgW = img.cols;
	int imgH = img.rows;
	//将图像显示在控件的中心
	int drawX = (int)(picCtrlW-imgW)/2;
	int drawY = (int)(picCtrlH-imgH)/2;

	MatToCImage(img, cimg);
	cimg.Draw(hDC,rect);	
	ReleaseDC(hDlg, hDC);

}