
// FindFileMultiThreadDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "InfoApp.h"

#define WM_LIST_ADD (WM_USER+123)
#define WM_TREADCOUNT_FRESH (WM_USER+124)
#define WM_FINDCOUNT_FRESH (WM_USER+125)
//#define WM_INFO_FRESH (WM_USER+126)
class CFileItem								
{
public:
	CString m_Path;	//查找路径
	CString m_Name;	//查找文件名
	HWND m_hMainDlg;//主对话框句柄
	CFileItem()
	{
		m_Path.Empty();
		m_Name.Empty();
		m_hMainDlg = NULL;
	}
	CFileItem(CFileItem& fItem)
	{
		m_Path = fItem.m_Path;
		m_Name = fItem.m_Name;
		m_hMainDlg = fItem.m_hMainDlg;
	}
};


// CFindFileMultiThreadDlg 对话框
class CFindFileMultiThreadDlg : public CDialogEx
{
// 构造
public:
	CFindFileMultiThreadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FINDFILEMULTITHREAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;									
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButtonStart();
	afx_msg LRESULT AddListItem(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT FreshTreadCount(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT FreshFindCount(WPARAM wParam,LPARAM lParam);
	static UINT __cdecl FindProc( LPVOID pParam );								//线程服务函数，递归

	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnSizeMainWndCtrl(int cx,int cy);
public:
	CRect m_rect;
	CSize m_size;
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOpenpath();													//菜单选项“打开路径”的响应函数
	afx_msg void OnCopypath();													//菜单选项“拷贝路径”至剪切板的响应函数
	afx_msg void OnOpenfile();													//菜单选项“打开文件”的响应函数
	CString m_strPath;
	
	virtual BOOL DestroyWindow();
	virtual void OnOK();
	void TestUSN();
	afx_msg void OnBnClickedUsnFunc();
};

extern CWinThread *pInfoDlgThread;
