// LogViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LogViewer.h"
#include "LogViewerDlg.h"
#include "ExpParse.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogViewerDlg dialog

CLogViewerDlg::CLogViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogViewerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogViewerDlg)
	DDX_Control(pDX, IDC_COMBO_MONTH2, m_ctrlComboMonth2);
	DDX_Control(pDX, IDC_COMBO_DAY2, m_ctrlComboDay2);
	DDX_Control(pDX, IDC_LIST_TEXT, m_ctrlListText);
	DDX_Control(pDX, IDC_COMBO_DAY, m_ctrlComboDay);
	DDX_Control(pDX, IDC_COMBO_MONTH, m_ctrlComboMonth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLogViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CLogViewerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, OnSelchangeComboMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_DAY, OnSelchangeComboDay)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TEXT, &CLogViewerDlg::OnLvnItemchangedListText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogViewerDlg message handlers

BOOL CLogViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgPtr(this);

	// 각 콤보박스들의 셀을 0 으로 초기화
	m_ctrlComboMonth.SetCurSel(0);
	m_ctrlComboDay.SetCurSel(0);

	m_ctrlComboMonth2.SetCurSel(0);
	m_ctrlComboDay2.SetCurSel(0);


	ColumnInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLogViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLogViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLogViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLogViewerDlg::OnOK() 
{
	// TODO: Add extra validation here


	CDialog::OnOK();
}

void CLogViewerDlg::OnBtnOpen() 
{
	// TODO: Add your control notification handler code here
	// 사용자가 날짜의 기간을 제대로 입력했는지를 체크한다
	if( m_ctrlComboMonth.GetCurSel() > m_ctrlComboMonth2.GetCurSel() )
	{
		MessageBox("Please enter the period correctly");
		return;
	}
	
	if( m_ctrlComboMonth.GetCurSel() == m_ctrlComboMonth2.GetCurSel() )
	{
		if(m_ctrlComboDay.GetCurSel() > m_ctrlComboDay2.GetCurSel() )
		{
			MessageBox("Please enter the period correctly");
			return;
		}
		return;
	}

	// 이상이 없으므로 실행한다
	CFileDialog dlg(FALSE, "txt", "*.txt",OFN_HIDEREADONLY , 
		"TEXT File(*.txt)|*.txt|All File(*.*)|*.*||");

	dlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;

	if(dlg.DoModal() == IDOK)
	{
		// 아래의 while 루프에서 pos변수자체가 사라지는 기현상이 발생하여 static으로 지정
		POSITION pos;
		CString filename;
		pos = dlg.GetStartPosition();

		// 첫번째 날짜를 date에 저장
		char date[6];
		memset(date, 0L, sizeof(date));
		wsprintf(date, "%02d.%02d",	m_ctrlComboMonth.GetCurSel() + 1, m_ctrlComboDay.GetCurSel() + 1);

		// 두번째 날짜를 date2에 저장
		char date2[6];
		memset(date2, 0L, sizeof(date2));
		wsprintf(date2, "%02d.%02d", m_ctrlComboMonth2.GetCurSel() + 1, m_ctrlComboDay2.GetCurSel() + 1);

		while(pos)
		{
			filename = dlg.GetNextPathName(pos);
			ExpParseMain((char*)(LPCTSTR)filename, date, date2);


		}

		return;
	}
}

void CLogViewerDlg::AddItem(int row, int colnum, char * pStr)
{
	// 아이템을 추가하는 부분
	LV_ITEM lvitem;
	lvitem.iItem = m_ctrlListText.GetItemCount();
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;
	lvitem.iImage = 0;
//	lvitem.stateMask = LVIS_STATEMASK;
//	lvitem.state = INDEXTOSTATEMAKS(1);
	lvitem.pszText = "";

	// 입력하려는 열이 없을경우 추가
	if(lvitem.iItem < row)
		m_ctrlListText.InsertItem(&lvitem);
	m_ctrlListText.SetItemText(row - 1, colnum, pStr);

}

void CLogViewerDlg::ClearAll()
{
	m_ctrlListText.DeleteAllItems();
}

void CLogViewerDlg::ColumnInit()
{
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt =  LVCFMT_CENTER;

	// 각 칼럼의 입력
	for(int i = 0; i < 16; i++)
	{
		lvcolumn.iSubItem = i;
		switch(i)
		{
		case 0:
			lvcolumn.cx = 100;
			lvcolumn.pszText = "hour";
			break;
		case 1:
			lvcolumn.cx = 100;
			lvcolumn.pszText = "Experience";
			break;
		case 2:
			lvcolumn.cx = 100;
			lvcolumn.pszText = "fame";
			break;
		case 3:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Crapple";
			break;
		case 4:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Swordmanship";
			break;
		case 5:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Archery";
			break;
		case 6:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Fencing";
			break;
		case 7:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Mace Fighting";
			break;
		case 8:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Pierce";
			break;
		case 9:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Whirl";
			break;
		case 10:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Hurl";
			break;
		case 11:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Parrying";
			break;
		case 12:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Double Swordmanship";
			break;
		case 13:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Double Mace Fighting";
			break;
		case 14:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Magery";
			break;
		case 15:
			lvcolumn.cx = 70;
			lvcolumn.pszText = "Orison";
			break;
		default:
			break;
		}
		m_ctrlListText.InsertColumn(i, &lvcolumn);

	}

}

void CLogViewerDlg::OnSelchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	// 잠시 아래의 기능을 막아놓자
//	 m_ctrlComboMonth2.SetCurSel(m_ctrlComboMonth.GetCurSel());
}

void CLogViewerDlg::OnSelchangeComboDay() 
{
	// TODO: Add your control notification handler code here
//	 m_ctrlComboDay2.SetCurSel(m_ctrlComboDay.GetCurSel());
//	_mkdir("c:\\new/do/assdf");	
}


void CLogViewerDlg::OnLvnItemchangedListText(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
