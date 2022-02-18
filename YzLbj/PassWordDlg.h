#pragma once


// CPassWordDlg 对话框

class CPassWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPassWordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSWORD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
														// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//afx_msg void OnPaint();
	//afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	CRect RectSmall;
	CRect RectLarge;
	CString m_laomima;
	CString m_xinmima;
	CString m_querenxinmima;

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	CString m_mima;
};
