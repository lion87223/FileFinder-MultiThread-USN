#pragma once


// CInfoDlg �Ի���

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoDlg();

// �Ի�������
	enum { IDD = IDD_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT FreshInfo(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OffSetWnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	virtual void PostNcDestroy();
};
