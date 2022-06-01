
// FindFileMultiThread.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "FindFileMultiThread.h"
#include "FindFileMultiThreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFindFileMultiThreadApp

BEGIN_MESSAGE_MAP(CFindFileMultiThreadApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFindFileMultiThreadApp 构造
CFindFileMultiThreadApp::CFindFileMultiThreadApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// 唯一的一个 CFindFileMultiThreadApp 对象
CFindFileMultiThreadApp theApp;

// CFindFileMultiThreadApp 初始化
BOOL CFindFileMultiThreadApp::InitInstance()
{
	CFindFileMultiThreadDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	return FALSE;
}

int CFindFileMultiThreadApp::ExitInstance()
{
	if(pInfoDlgThread != NULL)
		::PostThreadMessage( pInfoDlgThread->m_nThreadID,WM_QUIT,0,0);	//注意要向界面线程发送WM_QUIT消息，因为它在OnInitInstance中被强行打开了消息泵。不能自动关闭。
	return CWinApp::ExitInstance();
}
