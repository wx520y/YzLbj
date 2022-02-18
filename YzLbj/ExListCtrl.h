#if !defined(AFX_EXLISTCTRL_H__BA193F08_B72E_4CDF_BA6B_C169B895C25E__INCLUDED_)
#define AFX_EXLISTCTRL_H__BA193F08_B72E_4CDF_BA6B_C169B895C25E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListCtrl.h : header file
//	˵��������չ��CListCtrl��
//	֧�֣�
//		1. ����Ĭ�ϱ�����ɫ��Ĭ��������ɫ��Ĭ��ѡ��������ɫ��Ĭ��ѡ�б�����ɫ��������ɫ��
//		2. �����ض���Ԫ���������ɫ��������ɾ���л���ʱ���ض���Ԫ�����ɫ���䡣
//		3. ����Ĭ�����壨���е�Ԫ������һ�£�
//		4. ����ÿ����Ԫ��߿�
//		5. �����иߣ��������и���������߶�
//
//	��֧�֣�
//		1. �����Ѿ�����list���������
//		2. ͼ����ͼ���б���ͼ��������Ϊ������ͼ��
//		3. �ȵȡ�
//
//	ʹ�÷�����
//		1. ͨ��InsertItem(), DeleteItem(), DeleteAllItems(), InsertColumn(), DeleteColumn(),
//			���ɾ���к��У�ʹ�÷�����CListCtrlһ�¡������趨��ɫ����
//		2. ͨ��SetSubItemColor()����ĳһ��Ԫ����ɫ��
//		3. ͨ��SetItemHeight()�����и�
//		4. ͨ��SetDefaultClr()����Ĭ����ɫ
//
//	ԭ���飺
//		����Ĭ����ɫ���Ƶ�Ԫ�񣬲�ά��һ��CArray�����кź��к�Ϊ�������洢��Щ�ض��ĵ�Ԫ�ĵ���ɫ��Ϣ����Ĭ����ɫ��һ�µģ�
//		�������У����У�ʱ�������������ɫ���洢��ɾ���л���ʱ������������ɫ������о�ɾ��������ǰ��ɾ����Carray����
//		������ ����һ�����ݵ�3�У���ǰ�ĵ�3�б�ɵ�4�У�Carray�洢���к�>=3�������к�Ҫ��1��
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

//����item����ʽ
struct SUBITEMSTYLE 
{
	int				nRowIndex;								//�е�����
	int				nColumnIndex;							//�е�����
	COLORREF		clrBack;								//������ɫ
	COLORREF		clrText;								//������ɫ
	COLORREF		clrSelBack;								//ѡ�б�����ɫ
	COLORREF		clrSelText;								//ѡ��������ɫ
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
	//��ʽ�б�Ĳ���
	BOOL			IsNeedAddStyle(COLORREF clrBack, COLORREF clrText,COLORREF clrSelBack, COLORREF clrSelText);
	void			AddStyle(int rowindex, int columnindex, COLORREF clrBack, COLORREF clrText,COLORREF clrSelBack, COLORREF clrSelText);
	void			DelStyle(int rowindex, int columnindex);
	void			RollAry(int index, BOOL bRow, BOOL bUp);
private:
	int				m_nHeight;					//�и�
	CFont			m_Font;						//����
	COLORREF		m_clrBackDefault;			//Ĭ�ϱ�����ɫ
	COLORREF		m_clrTextDefault;			//Ĭ��������ɫ
	COLORREF		m_clrSelBackDefault;		//Ĭ��ѡ�б�����ɫ
	COLORREF		m_clrSelTextDefault;		//Ĭ��ѡ��������ɫ

	//Item����ʽ  ֻ�洢�������ʽ��ͨ���к��е��������в��ң���û�������Ĭ�ϵ���ʽ
	CArray<SUBITEMSTYLE, SUBITEMSTYLE&> m_arySubItemStyle;	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXLISTCTRL_H__BA193F08_B72E_4CDF_BA6B_C169B895C25E__INCLUDED_)
