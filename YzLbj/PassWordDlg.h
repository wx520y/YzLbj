#pragma once


// CPassWordDlg �Ի���

class CPassWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassWordDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSWORD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
														// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
