
// FindFileMultiThread.cpp : ����Ӧ�ó��������Ϊ��
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


// CFindFileMultiThreadApp ����
CFindFileMultiThreadApp::CFindFileMultiThreadApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// Ψһ��һ�� CFindFileMultiThreadApp ����
CFindFileMultiThreadApp theApp;

// CFindFileMultiThreadApp ��ʼ��
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
		::PostThreadMessage( pInfoDlgThread->m_nThreadID,WM_QUIT,0,0);	//ע��Ҫ������̷߳���WM_QUIT��Ϣ����Ϊ����OnInitInstance�б�ǿ�д�����Ϣ�á������Զ��رա�
	return CWinApp::ExitInstance();
}
