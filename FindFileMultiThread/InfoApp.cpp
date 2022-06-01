// InfoApp.cpp : 实现文件
//

#include "stdafx.h"
#include "FindFileMultiThread.h"
#include "InfoApp.h"
#include "InfoDlg.h"
#include "FindFileMultiThreadDlg.h"
#include "FindFileMultiThread.h"


// CInfoApp

IMPLEMENT_DYNCREATE(CInfoApp, CWinThread)

CInfoApp::CInfoApp()
{
}

CInfoApp::~CInfoApp()
{
}

BOOL CInfoApp::InitInstance()
{
	//ASSERT(::IsWindow(m_hParentWnd)); 
	CWnd* pParent = CWnd::FromHandle(m_hParentWnd);		
	CInfoDlg *pInfoDlg = new CInfoDlg();
	pInfoDlg->Create(IDD_INFO,NULL);				//特殊处理。为了将主窗口设置为新建线程窗口的父窗口。

	CRect rect1;
	AfxGetApp()->m_pMainWnd->GetWindowRect(rect1);

	CRect rect;
	rect.bottom = rect1.bottom -40;
	rect.top = rect.bottom - 40;
	rect.left = rect1.left +10;
	rect.right = rect.left + 750;
	pInfoDlg->MoveWindow(rect);
	
	pInfoDlg->ShowWindow(SW_SHOW);
	pInfoDlg->SetDlgItemText(IDC_INFO,_T(""));
	this->m_pMainWnd = pInfoDlg;

	pInfoDlg->RunModalLoop();
	return TRUE;
}

int CInfoApp::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CInfoApp, CWinThread)
END_MESSAGE_MAP()


// CInfoApp 消息处理程序
