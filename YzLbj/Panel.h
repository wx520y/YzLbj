#pragma once


// CPanel 对话框

class CPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CPanel)

public:
	CPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPanel();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_BOOL_LockIng;
	afx_msg void OnBnClickedCheckLockimg();
	afx_msg void OnBnClickedCancel();
};
