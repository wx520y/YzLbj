#if !defined(AFX_EXLISTCTRL_H__BA193F08_B72E_4CDF_BA6B_C169B895C25E__INCLUDED_)
#define AFX_EXLISTCTRL_H__BA193F08_B72E_4CDF_BA6B_C169B895C25E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListCtrl.h : header file
//	说明：（拓展的CListCtrl）
//	支持：
//		1. 设置默认背景颜色，默认字体颜色，默认选中字体颜色，默认选中背景颜色（四种颜色）
//		2. 设置特定单元格的四种颜色，当插入删除行或列时，特定单元格的颜色不变。
//		3. 设置默认字体（所有单元格字体一致）
//		4. 绘制每个单元格边框
//		5. 设置行高，并根据行高设置字体高度
//
//	不支持：
//		1. 排序（已经设置list风格，无排序）
//		2. 图标视图和列表视图（已设置为报表视图）
//		3. 等等。
//
//	使用方法：
//		1. 通过InsertItem(), DeleteItem(), DeleteAllItems(), InsertColumn(), DeleteColumn(),
//			添加删除行和列，使用方法和CListCtrl一致。（可设定颜色）。
//		2. 通过SetSubItemColor()设置某一单元格颜色。
//		3. 通过SetItemHeight()设置行高
//		4. 通过SetDefaultClr()设置默认颜色
//
//	原理简介：
//		根据默认颜色绘制单元格，并维护一个CArray，以行号和列号为索引，存储那些特定的单元的的颜色信息（与默认颜色不一致的）
//		当插入行（或列）时，如果是特殊颜色，存储。删除行或列时，查找特殊颜色，如果有就删除。插入前和删除后Carray滚号
//		（例如 插入一条数据到3行，以前的第3行变成第4行，Carray存储的行号>=3的数据行号要加1）
//
//
//	Author: xbm
//	Date:	2013-11-01
/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

#define DEFAULT_BACK_CLR		RGB(255, 255, 240)
#define DEFAULT_TEXT_CLR		RGB(  0,  0,    0)
#define DEFAULT_SEL_BACK_CLR	RGB(245, 245, 245)
#define DEFAULT_SEL_TEXT_CLR	RGB(  0,  0,    0)	
/////////////////////////////////////////////////////////////////////////////
// CExListCtrl window

//单个item的样式
struct SUBITEMSTYLE 
{
	int				nRowIndex;								//行的索引
	int				nColumnIndex;							//列的索引
	COLORREF		clrBack;								//背景颜色
	COLORREF		clrText;								//字体颜色
	COLORREF		clrSelBack;								//选中背景颜色
	COLORREF		clrSelText;								//选中字体颜色
};

class CExListCtrl : public CListCtrl
{
// Construction
public:
	CExListCtrl();

// Operations
public:
	void		SetItemHeight(int nHeight);		
	void		SetDefaultClr(COLORREF clrBack = DEFAULT_BACK_CLR, 
							  COLORREF clrText = DEFAULT_TEXT_CLR,
							  COLORREF clrSelBack = DEFAULT_SEL_BACK_CLR, 
							  COLORREF clrSelText = DEFAULT_SEL_TEXT_CLR );
	int			InsertItem(int nItem, LPCTSTR lpszItem,  
						   COLORREF clrBack = NULL, 
						   COLORREF clrText = NULL,
						   COLORREF clrSelBack = NULL, 
						   COLORREF clrSelText = NULL );
	BOOL		DeleteItem(int nItm);
	BOOL		DeleteAllItems();
	int			InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
							 int nFormat = LVCFMT_LEFT,
							 int nWidth = -1,
							 COLORREF clrBack = NULL, 
							 COLORREF clrText = NULL,
							 COLORREF clrSelBack = NULL, 
							 COLORREF clrSelText = NULL,
						 	 int nSubItem = -1 );
	BOOL		DeleteColumn (int nCol);
	void		SetSubItemColor(int rowindex, int clmindex,
								COLORREF clrBack = NULL, 
								COLORREF clrText = NULL,
								COLORREF clrSelBack = NULL, 
								COLORREF clrSelText = NULL);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListCtrl)
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void PreSubclassWindow();

// Implementation
public:
	virtual ~CExListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void			InitFont();
	BOOL			GetSubItemStyle(int rowindex, int columnindex, SUBITEMSTYLE& subitemstle);
	//样式列表的操作
	BOOL			IsNeedAddStyle(COLORREF clrBack, COLORREF clrText,COLORREF clrSelBack, COLORREF clrSelText);
	void			AddStyle(int rowindex, int columnindex, COLORREF clrBack, COLORREF clrText,COLORREF clrSelBack, COLORREF clrSelText);
	void			DelStyle(int rowindex, int columnindex);
	void			RollAry(int index, BOOL bRow, BOOL bUp);
private:
	int				m_nHeight;					//行高
	CFont			m_Font;						//字体
	COLORREF		m_clrBackDefault;			//默认背景颜色
	COLORREF		m_clrTextDefault;			//默认字体颜色
	COLORREF		m_clrSelBackDefault;		//默认选中背景颜色
	COLORREF		m_clrSelTextDefault;		//默认选中字体颜色

	//Item的样式  只存储特殊的样式，通过行和列的索引进行查找，若没有则采用默认的样式
	CArray<SUBITEMSTYLE, SUBITEMSTYLE&> m_arySubItemStyle;	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXLISTCTRL_H__BA193F08_B72E_4CDF_BA6B_C169B895C25E__INCLUDED_)
