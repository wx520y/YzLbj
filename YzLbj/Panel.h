#pragma once


// CPanel �Ի���

class CPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CPanel)

public:
	CPanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPanel();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_BOOL_LockIng;
	afx_msg void OnBnClickedCheckLockimg();
	afx_msg void OnBnClickedCancel();
};
