// ExListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ExListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl

CExListCtrl::CExListCtrl()
{
	m_nHeight = 25;						
	InitFont();
	m_clrBackDefault = DEFAULT_BACK_CLR;
	m_clrTextDefault = DEFAULT_TEXT_CLR;
	m_clrSelBackDefault = DEFAULT_SEL_BACK_CLR;	
	m_clrSelTextDefault = DEFAULT_SEL_TEXT_CLR;	
	m_arySubItemStyle.RemoveAll();
}

CExListCtrl::~CExListCtrl()
{
	m_arySubItemStyle.RemoveAll();
}


BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl message handlers

void CExListCtrl::PreSubclassWindow()
{
	ModifyStyle(LVS_SORTASCENDING|LVS_SORTDESCENDING,LVS_OWNERDRAWFIXED|LVS_REPORT);//修改样式
	CListCtrl::PreSubclassWindow();
}

void CExListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);						//设备上下文
	CFont* pOldFont = pDC->SelectObject(&m_Font);				//选中字体

	int rowIndex = lpDIS->itemID;								//行索引
	int columncount = GetHeaderCtrl()->GetItemCount();			//列数

	if (lpDIS->itemAction & ODA_DRAWENTIRE)						//需要重绘
	{
		for (int i=0; i<columncount; i++)						//循环所有的列
		{
			CString strText = GetItemText(rowIndex, i);			//文本
			CRect	ItemRect;									//一个单元格的矩形框
			GetSubItemRect(rowIndex, i, LVIR_LABEL, ItemRect);
			SUBITEMSTYLE subitemstyl;
			GetSubItemStyle(rowIndex, i, subitemstyl);			//获取样式

			if (lpDIS->itemState & ODS_SELECTED)				//选中状态
			{
				pDC->FillRect(ItemRect, &CBrush(subitemstyl.clrSelBack));		//背景色
				pDC->SetTextColor(subitemstyl.clrSelText);						//字体颜色			
			}
			else												//非选中状态
			{
				pDC->FillRect(ItemRect, &CBrush(subitemstyl.clrBack));		//背景色
				pDC->SetTextColor(subitemstyl.clrText);						//字体颜色	
			}
			//pDC->DrawEdge(ItemRect, EDGE_ETCHED, BF_RECT);					//边框 需要边框时，加
			pDC->DrawText(strText, lstrlen(strText), &ItemRect, DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE);
		}
	}

	pDC->SelectObject(pOldFont);
}

void CExListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CExListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = m_nHeight;// (m_nHeight即你想要的高度)
}

//设置行高，同时设置字体高度，行高的一半
void CExListCtrl::SetItemHeight(int nHeight)
{
	m_nHeight = nHeight;
	
	CRect rcWin;
	
	GetWindowRect(&rcWin);	
	WINDOWPOS wp;	
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();	
	wp.cy = rcWin.Height();	
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;	
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);

	InitFont();
}

//设置字体，高度为行高的一半
void CExListCtrl::InitFont()
{
	m_Font.DeleteObject();
	LOGFONT   log;  
	GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(log),&log);    
	log.lfHeight=m_nHeight/2;						//通过行高设置字高
	log.lfCharSet=GB2312_CHARSET;  
 	lstrcpy(log.lfFaceName,_T("宋体"));  
	m_Font.CreateFontIndirect(&log);
}

//根据行和列的索引，查找样式，找到了返回TRUE，否则返回FALSE
BOOL CExListCtrl::GetSubItemStyle(int rowindex, int columnindex, SUBITEMSTYLE& subitemstle)
{
	if (m_arySubItemStyle.GetSize()>0 && rowindex>=0 && columnindex>=0)
	{
		for (int i=0; i<m_arySubItemStyle.GetSize(); i++)
		{
			if (m_arySubItemStyle[i].nRowIndex == rowindex 
				&& m_arySubItemStyle[i].nColumnIndex == columnindex)
			{
				subitemstle.clrBack = m_arySubItemStyle[i].clrBack;
				subitemstle.clrText = m_arySubItemStyle[i].clrText;
				subitemstle.clrSelBack = m_arySubItemStyle[i].clrSelBack;
				subitemstle.clrSelText = m_arySubItemStyle[i].clrSelText;
				return TRUE;
			}
		}

	}
	//没有找到，返回默认样式
	subitemstle.clrBack = m_clrBackDefault;
	subitemstle.clrText = m_clrTextDefault;
	subitemstle.clrSelBack = m_clrSelBackDefault;
	subitemstle.clrSelText = m_clrSelTextDefault;
	return FALSE;
}

//设置默认颜色（背景色、文字颜色，选中背景色，选中文字颜色）
void CExListCtrl::SetDefaultClr(COLORREF clrBack /* = DEFAULT_BACK_CLR */, 
								COLORREF clrText /* = DEFAULT_TEXT_CLR */, 
								COLORREF clrSelBack /* = DEFAULT_SEL_BACK_CLR */, 
								COLORREF clrSelText /* = DEFAULT_SEL_TEXT_CLR */)
{
	m_clrBackDefault = clrBack;
	m_clrTextDefault = clrText;
	m_clrSelTextDefault = clrSelText;
	m_clrSelBackDefault = clrSelBack; 
}

//插入行
int CExListCtrl::InsertItem(int nItem, LPCTSTR lpszItem,   
							 COLORREF clrBack /* = NULL */, 
							 COLORREF clrText /* = NULL */, 
							 COLORREF clrSelBack /* = NULL */, 
							 COLORREF clrSelText /* = NULL */ )
{
	if (IsNeedAddStyle(clrBack, clrText, clrSelBack, clrSelText))
	{
		//插入之前滚号
		RollAry(nItem, TRUE, FALSE);
		int cout = GetHeaderCtrl()->GetItemCount();
		for (int i=0; i<cout; i++)
		{
			AddStyle(nItem, i, clrBack, clrText, clrSelBack, clrSelText);
		}
	}
	return CListCtrl::InsertItem(nItem, lpszItem);
}

//判断是否需要添加样式，不是默认颜色时添加
BOOL CExListCtrl::IsNeedAddStyle(COLORREF clrBack, COLORREF clrText,COLORREF clrSelBack, COLORREF clrSelText)
{
	if (clrBack == NULL && clrText == NULL && clrSelBack == NULL && clrSelText == NULL)
	{
		return FALSE;
	}
	if (clrBack == m_clrBackDefault && clrText == m_clrTextDefault
		&& clrSelBack == m_clrSelBackDefault && clrSelText == m_clrSelTextDefault)
	{
		return FALSE;
	}	
	return TRUE;
}

//添加样式，如果已经存在则替换
void CExListCtrl::AddStyle(int rowindex, int columnindex, COLORREF clrBack, COLORREF clrText,COLORREF clrSelBack, COLORREF clrSelText)
{
	if (clrBack == NULL)
	{
		clrBack = m_clrBackDefault;
	}
	if (clrText == NULL)
	{
		clrText = m_clrTextDefault;
	}
	if (clrSelBack == NULL)
	{
		clrSelBack = m_clrSelBackDefault;
	}
	if (clrSelText == NULL)
	{
		clrSelText = m_clrSelTextDefault;
	}

	for (int i=0; i<m_arySubItemStyle.GetSize(); i++)
	{
		if (m_arySubItemStyle[i].nRowIndex == rowindex && m_arySubItemStyle[i].nColumnIndex == columnindex)
		{
			m_arySubItemStyle[i].clrBack = clrBack;
			m_arySubItemStyle[i].clrText = clrText;
			m_arySubItemStyle[i].clrSelBack = clrSelBack;
			m_arySubItemStyle[i].clrSelText = clrSelText;
			return;		
		}
	}

	//没有，添加一个
	SUBITEMSTYLE st;
	st.nRowIndex = rowindex;
	st.nColumnIndex = columnindex;
	st.clrBack = clrBack;
	st.clrText = clrText;
	st.clrSelBack = clrSelBack;
	st.clrSelText = clrSelText;
	m_arySubItemStyle.Add(st);
}

//删除样式
void CExListCtrl::DelStyle(int rowindex, int columnindex)
{
	int count = m_arySubItemStyle.GetSize();
	for (int i=count-1; i>=0; i--)
	{
		if (m_arySubItemStyle[i].nRowIndex == rowindex && m_arySubItemStyle[i].nColumnIndex == columnindex)
		{
			m_arySubItemStyle.RemoveAt(i);
			return;		
		}
	}

}

//样式列表滚动 开始的索引， 行或列，滚号方向，
//插入行、插入列时，方向False 删除行删除列时方向为TRUE
//从索引号比index大的开始，索引值-1(bUp=TRUE) 或+1(bUp=FALSE)
//插入之前翻滚，删除之后翻滚
void CExListCtrl::RollAry(int index, BOOL bRow, BOOL bUp)
{
	for (int i=0; i<m_arySubItemStyle.GetSize(); i++)
	{
		if (bRow)		//行号翻滚
		{
			if (m_arySubItemStyle[i].nRowIndex >= index)
			{
				if (bUp)
				{
					m_arySubItemStyle[i].nRowIndex -= 1;
				}
				else
				{
					m_arySubItemStyle[i].nRowIndex += 1;
				}
			}
		}
		else			//列号翻滚
		{
			if (m_arySubItemStyle[i].nColumnIndex >= index)
			{
				if (bUp)
				{
					m_arySubItemStyle[i].nColumnIndex -= 1;
				}
				else
				{
					m_arySubItemStyle[i].nColumnIndex += 1;
				}
			}
		}
	}
}

//删除行
BOOL CExListCtrl::DeleteItem(int nItm)
{
	int count = GetHeaderCtrl()->GetItemCount();
	for (int i=0; i<count; i++)
	{
		DelStyle(nItm, i);
	}
	//删除之后滚号
	RollAry(nItm, TRUE, TRUE);
	return CListCtrl::DeleteItem(nItm);
}

//删除所有行
BOOL CExListCtrl::DeleteAllItems()
{
	m_arySubItemStyle.RemoveAll();
	return CListCtrl::DeleteAllItems();
}

int CExListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, 
							  int nFormat /* = LVCFMT_LEFT */, 
							  int nWidth /* = -1 */, 
							  COLORREF clrBack /* = NULL */, 
							  COLORREF clrText /* = NULL */, 
							  COLORREF clrSelBack /* = NULL */, 
							  COLORREF clrSelText /* = NULL */, 
							  int nSubItem /* = -1 */ )
{
	if (IsNeedAddStyle(clrBack, clrText, clrSelBack, clrSelText))
	{
		//插入先滚号
		RollAry(nCol, FALSE, FALSE);
		int count = GetItemCount();
		for (int i=0; i<count; i++)
		{
			AddStyle(i, nCol, clrBack, clrText, clrSelBack, clrSelText);
		}	
	}
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

//删除列
BOOL CExListCtrl::DeleteColumn(int nCol)
{
	int count = GetItemCount();
	for (int i=count-1; i>=0; i--)
	{
		DelStyle(i, nCol);
	}

	//删除之后滚号
	RollAry(nCol, FALSE, TRUE);
	return CListCtrl::DeleteColumn(nCol);
}

//单独设置单元格颜色
void CExListCtrl::SetSubItemColor(int rowindex, int clmindex, 
								  COLORREF clrBack /* = NULL */, 
								  COLORREF clrText /* = NULL */, 
								  COLORREF clrSelBack /* = NULL */, 
								  COLORREF clrSelText /* = NULL */)
{
	if (IsNeedAddStyle(clrBack, clrText, clrSelBack, clrSelText))
	{
		AddStyle(rowindex, clmindex, clrBack, clrText, clrSelBack, clrSelText);
		RedrawItems(rowindex, rowindex);	//	重绘
	}
}