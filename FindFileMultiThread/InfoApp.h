#pragma once



// CInfoApp

class CInfoApp : public CWinThread
{
	DECLARE_DYNCREATE(CInfoApp)

public:
	CInfoApp();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CInfoApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	HWND m_hParentWnd;			//��������
};


