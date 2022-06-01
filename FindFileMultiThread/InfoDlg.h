#pragma once


// CInfoDlg 对话框

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoDlg();

// 对话框数据
	enum { IDD = IDD_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT FreshInfo(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OffSetWnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	virtual void PostNcDestroy();
};
