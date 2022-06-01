


// FindFileMultiThreadDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "FindFileMultiThread.h"
#include "FindFileMultiThreadDlg.h"
#include "afxdialogex.h"
#include "InfoApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFindFileMultiThreadDlg �Ի���
CFindFileMultiThreadDlg::CFindFileMultiThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindFileMultiThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindFileMultiThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CFindFileMultiThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CFindFileMultiThreadDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFindFileMultiThreadDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFindFileMultiThreadDlg::OnBnClickedButtonStop)
	ON_MESSAGE(WM_LIST_ADD,AddListItem)
	ON_MESSAGE(WM_TREADCOUNT_FRESH,FreshTreadCount)
	ON_MESSAGE(WM_FINDCOUNT_FRESH,FreshFindCount)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CFindFileMultiThreadDlg::OnDblclkList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CFindFileMultiThreadDlg::OnRclickList)
	ON_COMMAND(ID_OPENPATH, &CFindFileMultiThreadDlg::OnOpenpath)
	ON_COMMAND(ID_COPYPATH, &CFindFileMultiThreadDlg::OnCopypath)
	ON_COMMAND(ID_OPENFILE, &CFindFileMultiThreadDlg::OnOpenfile)
	ON_BN_CLICKED(IDC_USN_FUNC, &CFindFileMultiThreadDlg::OnBnClickedUsnFunc)
END_MESSAGE_MAP()


// CFindFileMultiThreadDlg ��Ϣ�������
CWinThread *pInfoDlgThread = NULL;
BOOL CFindFileMultiThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//ShowWindow(SW_SHOWMAXIMIZED);  
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	//�����б���
	m_list.InsertColumn(0, _T("���ҽ��"), LVCFMT_LEFT, 650);

	CRect rect,rt;
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_LIST);			//�б�ؼ�
	pWnd->GetWindowRect(&rect);
	ScreenToClient(rect);
	GetClientRect(&rt);
	m_size.cx = rt.right - rect.right;		//��¼�б�ؼ���ԭʼλ�ã�Ϊ�ƶ����Ϸ�������ʱ���б�ؼ����ƶ����Ϸ�ê����׼�㣩
	m_size.cy = rt.bottom - rect.bottom;

#ifdef  _DEBUG
	SetDlgItemText(IDC_EDIT_PATH,_T("E:\\"));
	SetDlgItemText(IDC_EDIT_FILENAME,_T("pro"));
#endif
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CFindFileMultiThreadDlg::OnPaint(){
	CDialogEx::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFindFileMultiThreadDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}

LONG g_FindCount = 0;				//���ҵ������������ļ�����
LONG g_ThreadNum = 1;				//������Ŀ
BOOL g_bFinding = FALSE;			//�жϲ��ҵı�־
void CFindFileMultiThreadDlg::OnBnClickedButtonPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	BROWSEINFO mBroInfo = {0};							
	mBroInfo.hwndOwner = this->m_hWnd;
	ITEMIDLIST* pidl = SHBrowseForFolder(&mBroInfo);		//���ļ������
	if(SHGetPathFromIDList(pidl,szPath))
		SetDlgItemText(IDC_EDIT_PATH,szPath);
	CoTaskMemFree(pidl);
	OnKillFocus(GetDlgItem(IDC_BUTTON_PATH));
	GetDlgItem(IDC_BUTTON_START)->SetFocus( );
}
void CFindFileMultiThreadDlg::TestUSN()
{


	//m_strPath = _T("\\\\.\\E:");
	//HANDLE hVol = CreateFile(m_strPath,  0,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,  FILE_ATTRIBUTE_READONLY|FILE_FLAG_BACKUP_SEMANTICS , NULL); 
	//if(INVALID_HANDLE_VALUE==hVol){  
	//	TRACE("��ȡ�����̾��ʧ�� ���� handle:%x error:%d\n", hVol, GetLastError());  
	//	return;
	//}
	//DWORD br;  
	//CREATE_USN_JOURNAL_DATA cujd;  
	//cujd.MaximumSize = 0; // 0��ʾʹ��Ĭ��ֵ  
	//cujd.AllocationDelta = 0; // 0��ʾʹ��Ĭ��ֵ  
	//BOOL status = DeviceIoControl(hVol,FSCTL_CREATE_USN_JOURNAL,&cujd,sizeof(cujd),NULL,0,&br,NULL);  
 // 
	//if(0!=status){  
	//	int i = 0;
	//}else{  
	//	TRACE("��ʼ��USN��־�ļ�ʧ�� ���� status:%x error:%d\n", status, GetLastError());  
	//}  
}



void CFindFileMultiThreadDlg::OnBnClickedButtonStart()
{
	GetDlgItemText(IDC_EDIT_PATH,m_strPath);
	//TestUSN();
	//WIN32_FIND_DATA fileInfo;  //�����ļ���Ϣ�ṹ�����
	//HANDLE hFind;   
	//DWORD fileSize;   
	//CString fileName =  _T("E:\\bingPicture");   //Ŀ���ļ�·��
	//hFind = FindFirstFile(fileName ,&fileInfo);  // ��ȡ�ļ���Ϣ
	//if(hFind != INVALID_HANDLE_VALUE)   
	//	fileSize = fileInfo.nFileSizeLow;   //ȡ���ļ���С
	//char strFileSize[MAX_PATH];
	//_ultoa_s( fileSize, strFileSize, 10 );
	////MessageBox(NULL,strFileSize,"�ļ���С���ֽڣ�",MB_OK);
	//FindClose(hFind);//�رվ�����ͷ���Դ

	g_bFinding = TRUE;
	g_FindCount = 0;
	g_ThreadNum = 1;
	SetDlgItemInt(IDC_TREADCOUNT,1);
	SetDlgItemInt(IDC_FINDCOUNT,0);

	if(pInfoDlgThread)
		pInfoDlgThread->m_pMainWnd->SendMessage(WM_NCDESTROY,0,0);		//���֮ǰ�Ѿ�������һ�������߳���ʾɨ��·�����Ǿ��ȴݻ�֮ǰ�ġ�
	m_list.DeleteAllItems();							//����б�ؼ���һ�εĽ��
	
	CInfoApp *pThread = new CInfoApp();					//���������߳�
	pThread->CreateThread(CREATE_SUSPENDED);			//�ȹ��𣬵ȴ����Դ��Ľ����̸߳�ֵ��Ҫ��Ա������m_hParentWnd�������壩
	pThread->SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);
	pThread->m_bAutoDelete = TRUE;						//�߳��Զ��ر�
	//pThread->m_hParentWnd = this->m_hWnd;
	pThread->ResumeThread();

	pInfoDlgThread = pThread;
	SetTimer(1,100,NULL);

	CFileItem* pItem = new CFileItem();					//Ϊ���ҵݹ麯�������Ľṹ���࣬��������������Ϣ�������ļ��������������·���������ڵľ��(���̷߳���Ϣʹ)
	GetDlgItemText(IDC_EDIT_PATH,pItem->m_Path);
	GetDlgItemText(IDC_EDIT_FILENAME,pItem->m_Name);
	if(pItem->m_Path.IsEmpty() || pItem->m_Name.IsEmpty())
	{
		AfxMessageBox(_T("����·���������ļ�������д����"));
		return;
	}
	pItem->m_hMainDlg = this->m_hWnd;
	AfxBeginThread(FindProc,pItem,THREAD_PRIORITY_BELOW_NORMAL);						//���� ��һ�������̣߳����õݹ麯��
}

UINT __cdecl CFindFileMultiThreadDlg::FindProc( LPVOID pParam )		//���ĺ������̷߳��������ݹ���ʽ���ļ�ϵͳ����Ŀ¼��ֻҪû��ĩ�ң���㽨�����̡߳�
{
	Sleep(10);
	if(!g_bFinding)
		return 0;
	InterlockedIncrement(&g_ThreadNum);
	CFileItem* pItem = (CFileItem*)pParam;							//�Խ��ṹ���࣬�����洢��Ҫ���߳�(�ݹ麯��)�䴫�ݵ���Ϣ��

	//::PostMessage(pItem->m_hMainDlg,WM_TREADCOUNT_FRESH,0,0);		//���Ի�����Ϣ���̼߳�����1  ע��ԭ�򣺽��̹��죬��������Ϣ������
CString strTemp;
strTemp.Format(_T("%ld"),g_ThreadNum);
FromHandle(pItem->m_hMainDlg)->SetDlgItemText(IDC_TREADCOUNT,strTemp);
strTemp.Format(_T("%ld"),g_FindCount);
FromHandle(pItem->m_hMainDlg)->SetDlgItemText(IDC_FINDCOUNT,strTemp);		

	CString strDynaPath = pItem->m_Path;
	if( strDynaPath[strDynaPath.GetLength()-1] != _T('\\') )
		strDynaPath += _T("\\");

	CFileFind mFinder;												//���Ĳ�����
	BOOL bFind = mFinder.FindFile(strDynaPath + _T("*.*"));	
//	bFind = mFinder.FindNextFile();
	while(bFind )													//����·���������ļ�
	{
		bFind = mFinder.FindNextFile();
		if(mFinder.IsDots())
			continue;
		else if(mFinder.IsDirectory())	{						//������·����ͨ���ݹ麯��(�½��̷߳�����)����
			CFileItem* pSubItem = new CFileItem(*pItem);		//�����CFileItem�������ڲ�CFileItem���¾�CFileItemֻ��Ҫ�ı�·����
			pSubItem->m_Path = mFinder.GetFilePath();
			if(mFinder.GetFileName().MakeLower().Find(pItem->m_Name) >= 0){
				::SendMessage(pItem->m_hMainDlg,WM_LIST_ADD,0,(LPARAM)(LPCTSTR)mFinder.GetFilePath());
				InterlockedIncrement(&g_FindCount);				//��ͬ�̲߳�����һȫ�ֱ���g_FindCount��ԭ�ӻ���
			}
			CString* st = new CString(mFinder.GetRoot());		//ע������ʹ��PostMessage�Ļ�����Ҫnew����Ϊ�ֲ������������첽�����Ա��������ڵ�Ҫ��
			::SendMessage(pInfoDlgThread->m_pMainWnd->m_hWnd,INFO_DLG_FRESH,0,(LPARAM)(LPCTSTR)st);
			delete st;
			AfxBeginThread(FindProc,pSubItem);									
		}
		else {												  //�����ļ�
			CString strTraveName = mFinder.GetFileName();
			if(strTraveName.MakeLower().Find(pItem->m_Name) >= 0){
				CString strTravePath = mFinder.GetFilePath();
				::SendMessage(pItem->m_hMainDlg,WM_LIST_ADD,0,(LPARAM)(LPCTSTR)strTravePath);
				InterlockedIncrement(&g_FindCount);				//��ͬ�̲߳�����һȫ�ֱ���g_FindCount��ԭ�ӻ���
			}
		}
	//	::PostMessage(pItem->m_hMainDlg,WM_FINDCOUNT_FRESH,0,0);				// ע��ԭ�򣺽��̹��죬��������Ϣ������
	//FromHandle(pItem->m_hMainDlg)->SetDlgItemInt(IDC_FINDCOUNT,g_FindCount);	//ע��ԭ�򣺽��̹��죬������ˢ����̫��
	}

	InterlockedDecrement(&g_ThreadNum);							//�߳��˳���g_ThreadNum������һ
	//::PostMessage(pItem->m_hMainDlg,WM_TREADCOUNT_FRESH,0,0);
	//FromHandle(pItem->m_hMainDlg)->SetDlgItemInt(IDC_TREADCOUNT,g_ThreadNum);		// ע��ԭ�򣺽��̹��죬��������Ϣ������
	
	delete pItem;
	return 0;
}

LRESULT CFindFileMultiThreadDlg::AddListItem(WPARAM wParam,LPARAM lParam)
{
	CString strPath((LPCTSTR)lParam);
	m_list.InsertItem(m_list.GetItemCount(),strPath);
	//m_list.UnlockWindowUpdate();
	//m_list.SetRedraw(TRUE) ;
	//m_list.Invalidate();
	return TRUE;
}

LRESULT CFindFileMultiThreadDlg::FreshTreadCount(WPARAM wParam,LPARAM lParam)
{
	SetDlgItemInt(IDC_TREADCOUNT,g_ThreadNum);
	return TRUE;
}

LRESULT CFindFileMultiThreadDlg::FreshFindCount(WPARAM wParam,LPARAM lParam)
{
	SetDlgItemInt(IDC_FINDCOUNT,g_FindCount);
	return TRUE;
}

//LRESULT CFindFileMultiThreadDlg::FreshInforText(WPARAM wParam,LPARAM lParam)
//{
//	CString* st = (CString*)lParam;
//	SetDlgItemText(IDC_INFO,*st);
//
//
//	delete st;
//	return TRUE;
//}

void CFindFileMultiThreadDlg::OnSizeMainWndCtrl(int cx,int cy)
{
	CWnd* pWnd = GetDlgItem(IDC_LIST);
	if(!pWnd)
		return;

	CRect rect,rect1;	
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right = cx - m_size.cx;
	rect.bottom = cy - m_size.cy;
	pWnd->MoveWindow(rect);					//���ÿؼ���С
		
	int deltax = rect.Width();				//����CListCtrl�ؼ��п�
	for(int i = 0;i < 1;i++)
		m_list.SetColumnWidth(i,deltax);

	pWnd->GetWindowRect(&rect);				//CListCtrl�ؼ��µľ���
	ScreenToClient(&rect);

	pWnd = GetDlgItem(IDC_STATIC1);
	pWnd->GetWindowRect(&rect1);
	ScreenToClient(rect1);
	rect1.top = rect.bottom + 20;
	rect1.bottom = rect1.top + 20;
	pWnd->SetWindowPos(NULL,rect1.left,rect1.top,rect1.Width(),rect1.Height(),SWP_NOZORDER);		
	
	pWnd = GetDlgItem(IDC_TREADCOUNT);
	pWnd->GetWindowRect(&rect1);
	ScreenToClient(rect1);
	rect1.top = rect.bottom + 20;
	rect1.bottom = rect1.top + 20;
	pWnd->SetWindowPos(NULL,rect1.left,rect1.top,rect1.Width(),rect1.Height(),SWP_NOZORDER);		

	pWnd = GetDlgItem(IDC_STATIC2);
	pWnd->GetWindowRect(&rect1);
	ScreenToClient(rect1);
	rect1.top = rect.bottom + 20;
	rect1.bottom = rect1.top + 20;
	pWnd->SetWindowPos(NULL,rect1.left,rect1.top,rect1.Width(),rect1.Height(),SWP_NOZORDER);		

	pWnd = GetDlgItem(IDC_FINDCOUNT);
	pWnd->GetWindowRect(&rect1);
	ScreenToClient(rect1);
	rect1.top = rect.bottom + 20;
	rect1.bottom = rect1.top + 20;
	pWnd->SetWindowPos(NULL,rect1.left,rect1.top,rect1.Width(),rect1.Height(),SWP_NOZORDER);			
}

void CFindFileMultiThreadDlg::OnSize(UINT nType, int cx, int cy)					//�������ԶԻ���ʱ���Կؼ��Ĵ���
{
	CDialogEx::OnSize(nType, cx, cy);						
	if(!GetDlgItem(IDC_LIST))
		return;
	OnSizeMainWndCtrl(cx,cy);
	GetClientRect(&m_rect);					//��һ�ν���ʱ�������ڻ��Ƴɹ�������m_rect��ֵ

	if(pInfoDlgThread)
	{
		CWnd* p = pInfoDlgThread->m_pMainWnd;
		p->ShowWindow(SW_HIDE);
	}
	//::SendMessage(pInfoDlgThread->m_pMainWnd->m_hWnd,WM_SIZE,200,800);
	//::PostThreadMessage( pInfoDlgThread->m_nThreadID,WM_QUIT,0,0);	//ע��Ҫ������̷߳���WM_QUIT��Ϣ����Ϊ����OnInitInstance�б�ǿ�д�����Ϣ�á������Զ��رա�
}

void CFindFileMultiThreadDlg::OnBnClickedButtonStop()
{
	g_bFinding = FALSE;
}

void CFindFileMultiThreadDlg::OnTimer(UINT_PTR nIDEvent)
{	
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (g_ThreadNum > 1) {										//���Ի���ؼ�����
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		if(pInfoDlgThread && pInfoDlgThread->m_pMainWnd)
			//pInfoDlgThread->m_pMainWnd->SendMessage(WM_NCDESTROY,0,0);		
			pInfoDlgThread->m_pMainWnd->ShowWindow(SW_HIDE);
	}
	//pInfoDlgThread->m_pMainWnd->SetParent(this);
	//CString strTemp;
	//strTemp.Format(_T("%d"),g_ThreadNum);
	//SetDlgItemInt(IDC_TREADCOUNT,g_ThreadNum);
	//strTemp.Format(_T("%d"),g_FindCount);
	//SetDlgItemInt(IDC_FINDCOUNT,g_FindCount);
	CDialogEx::OnTimer(nIDEvent);
}

void CFindFileMultiThreadDlg::OnMove(int x, int y)		//�ƶ����Ի���ʱ���Կؼ��Ĵ���
{
	CDialogEx::OnMove(x, y);
	LONG lParamTemp = MAKELONG(x,y);

	if(pInfoDlgThread && pInfoDlgThread->m_pMainWnd)
		::SendMessage(pInfoDlgThread->m_pMainWnd->m_hWnd,INFO_DLG_OFFSET,0,(LPARAM)lParamTemp);		//�̴߳��ڶԻ����ƶ�

	// TODO: �ڴ˴������Ϣ����������
}

void CFindFileMultiThreadDlg::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nIndexRow = pNMItemActivate->iItem;

	CString strPath = m_list.GetItemText(nIndexRow,0);
	if(strPath.IsEmpty())
		return;
	CFileDialog fileDlg(TRUE , _T(" .*"),strPath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , _T("�����ļ�(*.*) |*.*||"),this  );
	fileDlg.m_ofn.lpstrInitialDir=strPath;											//��ʼ��·����
	if(fileDlg.DoModal() == IDOK) 
	{
		CString strFileName = fileDlg.GetPathName();
		ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW);
	}

}

void CFindFileMultiThreadDlg::OnRclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu* pContextMenu = menu.GetSubMenu(0);
	CPoint pt1;
	GetCursorPos(&pt1);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON ,pt1.x,pt1.y,this);

	int nIndexRow = pNMItemActivate->iItem;
	m_strPath = m_list.GetItemText(nIndexRow,0);
}

void CFindFileMultiThreadDlg::OnOpenpath()
{
	if(m_strPath.IsEmpty())
		return;
	CFileDialog fileDlg(TRUE , _T(" .*"),m_strPath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , _T("�����ļ�(*.*) |*.*||"),this  );
	fileDlg.m_ofn.lpstrInitialDir=m_strPath;											//��ʼ��·����
	if(fileDlg.DoModal() == IDOK) {
		CString strFileName = fileDlg.GetPathName();
		ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW);
	}
}

void CFindFileMultiThreadDlg::OnCopypath()
{
	if(m_strPath.IsEmpty())
		return;
	CString strPath = m_strPath;
	int nIndex = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(nIndex);

	if (!OpenClipboard()){								//��ϵͳ���а�
		AfxMessageBox(_T("�򿪼��а���Ч"));
		return;
	}
	if(!EmptyClipboard()){								//���ϵͳ���а�
		AfxMessageBox(_T("��ռ��а���Ч"));
		return;  
	}
	size_t cbStr = (strPath.GetLength() + 1) * sizeof(TCHAR);

	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);	//�����ڴ�
	memcpy_s(GlobalLock(hData), cbStr, strPath.LockBuffer(), cbStr);
	GlobalUnlock(hData);								//����Ŀ���ַ������ڴ�
	//strPath.UnlockBuffer();

	UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
	if (::SetClipboardData(uiFormat, hData) == NULL) 
		AfxMessageBox(_T("����ʧ��"));  
	else 
		AfxMessageBox(_T("���Ƴɹ���Ctrl+V��ճ��������Ӧ�ó���"));  
	CloseClipboard();
}


void CFindFileMultiThreadDlg::OnOpenfile()
{
	if(m_strPath.IsEmpty())
		return;
	ShellExecute(NULL, _T("open"), m_strPath, NULL, NULL, SW_SHOW);
}


BOOL CFindFileMultiThreadDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pInfoDlgThread)
		pInfoDlgThread->m_pMainWnd->SendMessage(WM_NCDESTROY,0,0);
	return CDialogEx::DestroyWindow();
}


void CFindFileMultiThreadDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedButtonStart();
}


void CFindFileMultiThreadDlg::OnBnClickedUsnFunc()
{
	CString strFileName,strSearchPath;
	GetDlgItemText(IDC_EDIT_FILENAME,strFileName);
	GetDlgItemText(IDC_EDIT_PATH,strSearchPath);
	if(strFileName.IsEmpty() || strSearchPath.IsEmpty())
	{
		AfxMessageBox(_T("����·���������ļ�������д����"));
		return;
	}
	g_FindCount = 0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();
	CDriveIndex* pStDi = NULL;
	pStDi = CreateIndex(_T('E'));
	BOOL b;
	CString strTotal = Search(pStDi,strFileName.GetBuffer(),strSearchPath.GetBuffer(),TRUE,FALSE,1024,&b);
	CStringArray* m_result = new CStringArray;
	while(TRUE)
	{
		int index = strTotal.Find(_T("\n"));
		if(index == -1)
		{
			m_result->Add(strTotal);
			break ;
		}
		CString test1 = strTotal.Left(index);
		m_list.InsertItem(m_list.GetItemCount(),test1);
		m_result->Add(test1);
		g_FindCount++;
		SetDlgItemInt(IDC_FINDCOUNT,g_FindCount);
		strTotal = strTotal.Right(strTotal.GetLength()-index-1);
	}

	DeleteIndex(pStDi);
}
