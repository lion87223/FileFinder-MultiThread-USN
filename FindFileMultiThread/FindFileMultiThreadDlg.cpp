


// FindFileMultiThreadDlg.cpp : 实现文件
#include "stdafx.h"
#include "FindFileMultiThread.h"
#include "FindFileMultiThreadDlg.h"
#include "afxdialogex.h"
#include "InfoApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFindFileMultiThreadDlg 对话框
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


// CFindFileMultiThreadDlg 消息处理程序
CWinThread *pInfoDlgThread = NULL;
BOOL CFindFileMultiThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//ShowWindow(SW_SHOWMAXIMIZED);  
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	// 执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	//增加列表风格
	m_list.InsertColumn(0, _T("查找结果"), LVCFMT_LEFT, 650);

	CRect rect,rt;
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_LIST);			//列表控件
	pWnd->GetWindowRect(&rect);
	ScreenToClient(rect);
	GetClientRect(&rt);
	m_size.cx = rt.right - rect.right;		//记录列表控件的原始位置（为移动和拖放主窗口时，列表控件的移动和拖放锚定基准点）
	m_size.cy = rt.bottom - rect.bottom;

#ifdef  _DEBUG
	SetDlgItemText(IDC_EDIT_PATH,_T("E:\\"));
	SetDlgItemText(IDC_EDIT_FILENAME,_T("pro"));
#endif
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CFindFileMultiThreadDlg::OnPaint(){
	CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFindFileMultiThreadDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}

LONG g_FindCount = 0;				//查找到符合条件的文件个数
LONG g_ThreadNum = 1;				//进程数目
BOOL g_bFinding = FALSE;			//中断查找的标志
void CFindFileMultiThreadDlg::OnBnClickedButtonPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	BROWSEINFO mBroInfo = {0};							
	mBroInfo.hwndOwner = this->m_hWnd;
	ITEMIDLIST* pidl = SHBrowseForFolder(&mBroInfo);		//打开文件浏览器
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
	//	TRACE("获取驱动盘句柄失败 —— handle:%x error:%d\n", hVol, GetLastError());  
	//	return;
	//}
	//DWORD br;  
	//CREATE_USN_JOURNAL_DATA cujd;  
	//cujd.MaximumSize = 0; // 0表示使用默认值  
	//cujd.AllocationDelta = 0; // 0表示使用默认值  
	//BOOL status = DeviceIoControl(hVol,FSCTL_CREATE_USN_JOURNAL,&cujd,sizeof(cujd),NULL,0,&br,NULL);  
 // 
	//if(0!=status){  
	//	int i = 0;
	//}else{  
	//	TRACE("初始化USN日志文件失败 —— status:%x error:%d\n", status, GetLastError());  
	//}  
}



void CFindFileMultiThreadDlg::OnBnClickedButtonStart()
{
	GetDlgItemText(IDC_EDIT_PATH,m_strPath);
	//TestUSN();
	//WIN32_FIND_DATA fileInfo;  //定义文件信息结构体变量
	//HANDLE hFind;   
	//DWORD fileSize;   
	//CString fileName =  _T("E:\\bingPicture");   //目标文件路径
	//hFind = FindFirstFile(fileName ,&fileInfo);  // 获取文件信息
	//if(hFind != INVALID_HANDLE_VALUE)   
	//	fileSize = fileInfo.nFileSizeLow;   //取得文件大小
	//char strFileSize[MAX_PATH];
	//_ultoa_s( fileSize, strFileSize, 10 );
	////MessageBox(NULL,strFileSize,"文件大小（字节）",MB_OK);
	//FindClose(hFind);//关闭句柄，释放资源

	g_bFinding = TRUE;
	g_FindCount = 0;
	g_ThreadNum = 1;
	SetDlgItemInt(IDC_TREADCOUNT,1);
	SetDlgItemInt(IDC_FINDCOUNT,0);

	if(pInfoDlgThread)
		pInfoDlgThread->m_pMainWnd->SendMessage(WM_NCDESTROY,0,0);		//如果之前已经创建了一个窗口线程显示扫描路径，那就先摧毁之前的。
	m_list.DeleteAllItems();							//清空列表控件航一次的结果
	
	CInfoApp *pThread = new CInfoApp();					//创建界面线程
	pThread->CreateThread(CREATE_SUSPENDED);			//先挂起，等待给自创的界面线程赋值重要成员变量：m_hParentWnd（主窗体）
	pThread->SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);
	pThread->m_bAutoDelete = TRUE;						//线程自动关闭
	//pThread->m_hParentWnd = this->m_hWnd;
	pThread->ResumeThread();

	pInfoDlgThread = pThread;
	SetTimer(1,100,NULL);

	CFileItem* pItem = new CFileItem();					//为查找递归函数创建的结构体类，用来保存三个信息：查找文件名、动变深入的路径、主窗口的句柄(分线程发消息使)
	GetDlgItemText(IDC_EDIT_PATH,pItem->m_Path);
	GetDlgItemText(IDC_EDIT_FILENAME,pItem->m_Name);
	if(pItem->m_Path.IsEmpty() || pItem->m_Name.IsEmpty())
	{
		AfxMessageBox(_T("搜索路径和所搜文件名请填写完整"));
		return;
	}
	pItem->m_hMainDlg = this->m_hWnd;
	AfxBeginThread(FindProc,pItem,THREAD_PRIORITY_BELOW_NORMAL);						//创建 第一个查找线程，启用递归函数
}

UINT __cdecl CFindFileMultiThreadDlg::FindProc( LPVOID pParam )		//核心函数，线程服务函数，递归形式。文件系统树形目录，只要没到末梢，层层建立新线程。
{
	Sleep(10);
	if(!g_bFinding)
		return 0;
	InterlockedIncrement(&g_ThreadNum);
	CFileItem* pItem = (CFileItem*)pParam;							//自建结构体类，用来存储需要在线程(递归函数)间传递的信息。

	//::PostMessage(pItem->m_hMainDlg,WM_TREADCOUNT_FRESH,0,0);		//主对话框消息：线程计数加1  注释原因：进程过快，主窗口消息泵阻塞
CString strTemp;
strTemp.Format(_T("%ld"),g_ThreadNum);
FromHandle(pItem->m_hMainDlg)->SetDlgItemText(IDC_TREADCOUNT,strTemp);
strTemp.Format(_T("%ld"),g_FindCount);
FromHandle(pItem->m_hMainDlg)->SetDlgItemText(IDC_FINDCOUNT,strTemp);		

	CString strDynaPath = pItem->m_Path;
	if( strDynaPath[strDynaPath.GetLength()-1] != _T('\\') )
		strDynaPath += _T("\\");

	CFileFind mFinder;												//核心查找器
	BOOL bFind = mFinder.FindFile(strDynaPath + _T("*.*"));	
//	bFind = mFinder.FindNextFile();
	while(bFind )													//遍历路径中所有文件
	{
		bFind = mFinder.FindNextFile();
		if(mFinder.IsDots())
			continue;
		else if(mFinder.IsDirectory())	{						//发现子路径，通过递归函数(新建线程服务函数)深入
			CFileItem* pSubItem = new CFileItem(*pItem);		//将外层CFileItem拷贝给内层CFileItem（新旧CFileItem只需要改变路径）
			pSubItem->m_Path = mFinder.GetFilePath();
			if(mFinder.GetFileName().MakeLower().Find(pItem->m_Name) >= 0){
				::SendMessage(pItem->m_hMainDlg,WM_LIST_ADD,0,(LPARAM)(LPCTSTR)mFinder.GetFilePath());
				InterlockedIncrement(&g_FindCount);				//不同线程操作用一全局变量g_FindCount：原子互锁
			}
			CString* st = new CString(mFinder.GetRoot());		//注意这里使用PostMessage的话，就要new。因为局部变量不满足异步函数对变量生存期的要求。
			::SendMessage(pInfoDlgThread->m_pMainWnd->m_hWnd,INFO_DLG_FRESH,0,(LPARAM)(LPCTSTR)st);
			delete st;
			AfxBeginThread(FindProc,pSubItem);									
		}
		else {												  //单个文件
			CString strTraveName = mFinder.GetFileName();
			if(strTraveName.MakeLower().Find(pItem->m_Name) >= 0){
				CString strTravePath = mFinder.GetFilePath();
				::SendMessage(pItem->m_hMainDlg,WM_LIST_ADD,0,(LPARAM)(LPCTSTR)strTravePath);
				InterlockedIncrement(&g_FindCount);				//不同线程操作用一全局变量g_FindCount：原子互锁
			}
		}
	//	::PostMessage(pItem->m_hMainDlg,WM_FINDCOUNT_FRESH,0,0);				// 注释原因：进程过快，主窗口消息泵阻塞
	//FromHandle(pItem->m_hMainDlg)->SetDlgItemInt(IDC_FINDCOUNT,g_FindCount);	//注释原因：进程过快，主窗口刷新新太快
	}

	InterlockedDecrement(&g_ThreadNum);							//线程退出，g_ThreadNum计数减一
	//::PostMessage(pItem->m_hMainDlg,WM_TREADCOUNT_FRESH,0,0);
	//FromHandle(pItem->m_hMainDlg)->SetDlgItemInt(IDC_TREADCOUNT,g_ThreadNum);		// 注释原因：进程过快，主窗口消息泵阻塞
	
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
	pWnd->MoveWindow(rect);					//设置控件大小
		
	int deltax = rect.Width();				//设置CListCtrl控件列宽
	for(int i = 0;i < 1;i++)
		m_list.SetColumnWidth(i,deltax);

	pWnd->GetWindowRect(&rect);				//CListCtrl控件新的矩形
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

void CFindFileMultiThreadDlg::OnSize(UINT nType, int cx, int cy)					//缩放主对对话框时，对控件的处理
{
	CDialogEx::OnSize(nType, cx, cy);						
	if(!GetDlgItem(IDC_LIST))
		return;
	OnSizeMainWndCtrl(cx,cy);
	GetClientRect(&m_rect);					//第一次进来时（主窗口绘制成功），对m_rect赋值

	if(pInfoDlgThread)
	{
		CWnd* p = pInfoDlgThread->m_pMainWnd;
		p->ShowWindow(SW_HIDE);
	}
	//::SendMessage(pInfoDlgThread->m_pMainWnd->m_hWnd,WM_SIZE,200,800);
	//::PostThreadMessage( pInfoDlgThread->m_nThreadID,WM_QUIT,0,0);	//注意要向界面线程发送WM_QUIT消息，因为它在OnInitInstance中被强行打开了消息泵。不能自动关闭。
}

void CFindFileMultiThreadDlg::OnBnClickedButtonStop()
{
	g_bFinding = FALSE;
}

void CFindFileMultiThreadDlg::OnTimer(UINT_PTR nIDEvent)
{	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_ThreadNum > 1) {										//主对话框控件处理
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

void CFindFileMultiThreadDlg::OnMove(int x, int y)		//移动主对话框时，对控件的处理
{
	CDialogEx::OnMove(x, y);
	LONG lParamTemp = MAKELONG(x,y);

	if(pInfoDlgThread && pInfoDlgThread->m_pMainWnd)
		::SendMessage(pInfoDlgThread->m_pMainWnd->m_hWnd,INFO_DLG_OFFSET,0,(LPARAM)lParamTemp);		//线程窗口对话框移动

	// TODO: 在此处添加消息处理程序代码
}

void CFindFileMultiThreadDlg::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nIndexRow = pNMItemActivate->iItem;

	CString strPath = m_list.GetItemText(nIndexRow,0);
	if(strPath.IsEmpty())
		return;
	CFileDialog fileDlg(TRUE , _T(" .*"),strPath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , _T("所有文件(*.*) |*.*||"),this  );
	fileDlg.m_ofn.lpstrInitialDir=strPath;											//初始化路径。
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
	CFileDialog fileDlg(TRUE , _T(" .*"),m_strPath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , _T("所有文件(*.*) |*.*||"),this  );
	fileDlg.m_ofn.lpstrInitialDir=m_strPath;											//初始化路径。
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

	if (!OpenClipboard()){								//打开系统剪切板
		AfxMessageBox(_T("打开剪切板无效"));
		return;
	}
	if(!EmptyClipboard()){								//清空系统剪切板
		AfxMessageBox(_T("清空剪切板无效"));
		return;  
	}
	size_t cbStr = (strPath.GetLength() + 1) * sizeof(TCHAR);

	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);	//申请内存
	memcpy_s(GlobalLock(hData), cbStr, strPath.LockBuffer(), cbStr);
	GlobalUnlock(hData);								//拷贝目标字符串至内存
	//strPath.UnlockBuffer();

	UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
	if (::SetClipboardData(uiFormat, hData) == NULL) 
		AfxMessageBox(_T("复制失败"));  
	else 
		AfxMessageBox(_T("复制成功。Ctrl+V可粘贴至其他应用程序。"));  
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
	// TODO: 在此添加专用代码和/或调用基类
	if(pInfoDlgThread)
		pInfoDlgThread->m_pMainWnd->SendMessage(WM_NCDESTROY,0,0);
	return CDialogEx::DestroyWindow();
}


void CFindFileMultiThreadDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedButtonStart();
}


void CFindFileMultiThreadDlg::OnBnClickedUsnFunc()
{
	CString strFileName,strSearchPath;
	GetDlgItemText(IDC_EDIT_FILENAME,strFileName);
	GetDlgItemText(IDC_EDIT_PATH,strSearchPath);
	if(strFileName.IsEmpty() || strSearchPath.IsEmpty())
	{
		AfxMessageBox(_T("搜索路径和所搜文件名请填写完整"));
		return;
	}
	g_FindCount = 0;
	// TODO: 在此添加控件通知处理程序代码
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
