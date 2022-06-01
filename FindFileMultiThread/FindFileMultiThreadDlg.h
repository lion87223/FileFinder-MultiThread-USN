
// FindFileMultiThreadDlg.h : ͷ�ļ�
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
	CString m_Path;	//����·��
	CString m_Name;	//�����ļ���
	HWND m_hMainDlg;//���Ի�����
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


// CFindFileMultiThreadDlg �Ի���
class CFindFileMultiThreadDlg : public CDialogEx
{
// ����
public:
	CFindFileMultiThreadDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FINDFILEMULTITHREAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	static UINT __cdecl FindProc( LPVOID pParam );								//�̷߳��������ݹ�

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
	afx_msg void OnOpenpath();													//�˵�ѡ���·��������Ӧ����
	afx_msg void OnCopypath();													//�˵�ѡ�����·���������а����Ӧ����
	afx_msg void OnOpenfile();													//�˵�ѡ����ļ�������Ӧ����
	CString m_strPath;
	
	virtual BOOL DestroyWindow();
	virtual void OnOK();
	void TestUSN();
	afx_msg void OnBnClickedUsnFunc();
};

extern CWinThread *pInfoDlgThread;
