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
	ModifyStyle(LVS_SORTASCENDING|LVS_SORTDESCENDING,LVS_OWNERDRAWFIXED|LVS_REPORT);//�޸���ʽ
	CListCtrl::PreSubclassWindow();
}

void CExListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);						//�豸������
	CFont* pOldFont = pDC->SelectObject(&m_Font);				//ѡ������

	int rowIndex = lpDIS->itemID;								//������
	int columncount = GetHeaderCtrl()->GetItemCount();			//����

	if (lpDIS->itemAction & ODA_DRAWENTIRE)						//��Ҫ�ػ�
	{
		for (int i=0; i<columncount; i++)						//ѭ�����е���
		{
			CString strText = GetItemText(rowIndex, i);			//�ı�
			CRect	ItemRect;									//һ����Ԫ��ľ��ο�
			GetSubItemRect(rowIndex, i, LVIR_LABEL, ItemRect);
			SUBITEMSTYLE subitemstyl;
			GetSubItemStyle(rowIndex, i, subitemstyl);			//��ȡ��ʽ

			if (lpDIS->itemState & ODS_SELECTED)				//ѡ��״̬
			{
				pDC->FillRect(ItemRect, &CBrush(subitemstyl.clrSelBack));		//����ɫ
				pDC->SetTextColor(subitemstyl.clrSelText);						//������ɫ			
			}
			else												//��ѡ��״̬
			{
				pDC->FillRect(ItemRect, &CBrush(subitemstyl.clrBack));		//����ɫ
				pDC->SetTextColor(subitemstyl.clrText);						//������ɫ	
			}
			//pDC->DrawEdge(ItemRect, EDGE_ETCHED, BF_RECT);					//�߿� ��Ҫ�߿�ʱ����
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
	lpMeasureItemStruct->itemHeight = m_nHeight;// (m_nHeight������Ҫ�ĸ߶�)
}

//�����иߣ�ͬʱ��������߶ȣ��иߵ�һ��
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

//�������壬�߶�Ϊ�иߵ�һ��
void CExListCtrl::InitFont()
{
	m_Font.DeleteObject();
	LOGFONT   log;  
	GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(log),&log);    
	log.lfHeight=m_nHeight/2;						//ͨ���и������ָ�
	log.lfCharSet=GB2312_CHARSET;  
 	lstrcpy(log.lfFaceName,_T("����"));  
	m_Font.CreateFontIndirect(&log);
}

//�����к��е�������������ʽ���ҵ��˷���TRUE�����򷵻�FALSE
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
	//û���ҵ�������Ĭ����ʽ
	subitemstle.clrBack = m_clrBackDefault;
	subitemstle.clrText = m_clrTextDefault;
	subitemstle.clrSelBack = m_clrSelBackDefault;
	subitemstle.clrSelText = m_clrSelTextDefault;
	return FALSE;
}

//����Ĭ����ɫ������ɫ��������ɫ��ѡ�б���ɫ��ѡ��������ɫ��
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

//������
int CExListCtrl::InsertItem(int nItem, LPCTSTR lpszItem,   
							 COLORREF clrBack /* = NULL */, 
							 COLORREF clrText /* = NULL */, 
							 COLORREF clrSelBack /* = NULL */, 
							 COLORREF clrSelText /* = NULL */ )
{
	if (IsNeedAddStyle(clrBack, clrText, clrSelBack, clrSelText))
	{
		//����֮ǰ����
		RollAry(nItem, TRUE, FALSE);
		int cout = GetHeaderCtrl()->GetItemCount();
		for (int i=0; i<cout; i++)
		{
			AddStyle(nItem, i, clrBack, clrText, clrSelBack, clrSelText);
		}
	}
	return CListCtrl::InsertItem(nItem, lpszItem);
}

//�ж��Ƿ���Ҫ�����ʽ������Ĭ����ɫʱ���
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

//�����ʽ������Ѿ��������滻
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

	//û�У����һ��
	SUBITEMSTYLE st;
	st.nRowIndex = rowindex;
	st.nColumnIndex = columnindex;
	st.clrBack = clrBack;
	st.clrText = clrText;
	st.clrSelBack = clrSelBack;
	st.clrSelText = clrSelText;
	m_arySubItemStyle.Add(st);
}

//ɾ����ʽ
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

//��ʽ�б���� ��ʼ�������� �л��У����ŷ���
//�����С�������ʱ������False ɾ����ɾ����ʱ����ΪTRUE
//�������ű�index��Ŀ�ʼ������ֵ-1(bUp=TRUE) ��+1(bUp=FALSE)
//����֮ǰ������ɾ��֮�󷭹�
void CExListCtrl::RollAry(int index, BOOL bRow, BOOL bUp)
{
	for (int i=0; i<m_arySubItemStyle.GetSize(); i++)
	{
		if (bRow)		//�кŷ���
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
		else			//�кŷ���
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

//ɾ����
BOOL CExListCtrl::DeleteItem(int nItm)
{
	int count = GetHeaderCtrl()->GetItemCount();
	for (int i=0; i<count; i++)
	{
		DelStyle(nItm, i);
	}
	//ɾ��֮�����
	RollAry(nItm, TRUE, TRUE);
	return CListCtrl::DeleteItem(nItm);
}

//ɾ��������
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
		//�����ȹ���
		RollAry(nCol, FALSE, FALSE);
		int count = GetItemCount();
		for (int i=0; i<count; i++)
		{
			AddStyle(i, nCol, clrBack, clrText, clrSelBack, clrSelText);
		}	
	}
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

//ɾ����
BOOL CExListCtrl::DeleteColumn(int nCol)
{
	int count = GetItemCount();
	for (int i=count-1; i>=0; i--)
	{
		DelStyle(i, nCol);
	}

	//ɾ��֮�����
	RollAry(nCol, FALSE, TRUE);
	return CListCtrl::DeleteColumn(nCol);
}

//�������õ�Ԫ����ɫ
void CExListCtrl::SetSubItemColor(int rowindex, int clmindex, 
								  COLORREF clrBack /* = NULL */, 
								  COLORREF clrText /* = NULL */, 
								  COLORREF clrSelBack /* = NULL */, 
								  COLORREF clrSelText /* = NULL */)
{
	if (IsNeedAddStyle(clrBack, clrText, clrSelBack, clrSelText))
	{
		AddStyle(rowindex, clmindex, clrBack, clrText, clrSelBack, clrSelText);
		RedrawItems(rowindex, rowindex);	//	�ػ�
	}
}