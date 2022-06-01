// InfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FindFileMultiThread.h"
#include "InfoDlg.h"
#include "afxdialogex.h"


// CInfoDlg �Ի���
IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	ON_MESSAGE(INFO_DLG_FRESH,FreshInfo)
	ON_MESSAGE(INFO_DLG_OFFSET,OffSetWnd)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CInfoDlg ��Ϣ�������


BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

LRESULT CInfoDlg::FreshInfo(WPARAM wParam,LPARAM lParam)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_INFO);
	pEdit->SetSel(0, -1);  // ѡ�������ַ�
	pEdit->Clear();      // ֻ���޷����
	SetDlgItemText(IDC_INFO,*(CString*)lParam);
	return TRUE;
}

LRESULT CInfoDlg::OffSetWnd(WPARAM wParam,LPARAM lParam)
{
	int xPos = (int)LOWORD(lParam) ;   // horizontal position 
	int yPos = (int)HIWORD(lParam) ;   // vertical position

	CRect rtMain;
	AfxGetApp()->m_pMainWnd->GetWindowRect(&rtMain);

	CRect rect;
	GetWindowRect(&rect);

	rect.bottom = rtMain.bottom -40;
	rect.top = rect.bottom - 40;
	//rect.top = rtMain.bottom - 120;
	//rect.bottom  = rect.top + 40;
	rect.left = rtMain.left +10;
	rect.right = rect.left + 750;		//����rect.leftһ�䣬rect.Width()�ͻ�仯������400ֻ���Ǹ���ֵ��
	SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);		//rect.OffsetRect(10,435)�Ƿ��̴߳�������������ƫ����	
	return TRUE;
}

void CInfoDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
}

void CInfoDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(this)
		delete this;
}


