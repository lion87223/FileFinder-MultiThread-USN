
// FindFileMultiThread.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFindFileMultiThreadApp:
// �йش����ʵ�֣������ FindFileMultiThread.cpp
//

class CFindFileMultiThreadApp : public CWinApp
{
public:
	CFindFileMultiThreadApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CFindFileMultiThreadApp theApp;