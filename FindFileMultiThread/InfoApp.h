#pragma once



// CInfoApp

class CInfoApp : public CWinThread
{
	DECLARE_DYNCREATE(CInfoApp)

public:
	CInfoApp();           // 动态创建所使用的受保护的构造函数
	virtual ~CInfoApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	HWND m_hParentWnd;			//父窗体句柄
};


