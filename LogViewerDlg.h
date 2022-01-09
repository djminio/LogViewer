// LogViewerDlg.h : header file
//

#if !defined(AFX_LOGVIEWERDLG_H__32D0C901_BF8B_42F6_8F96_EB8566DDC527__INCLUDED_)
#define AFX_LOGVIEWERDLG_H__32D0C901_BF8B_42F6_8F96_EB8566DDC527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLogViewerDlg dialog

class CLogViewerDlg : public CDialog
{
// Construction
public:
	void ColumnInit();
	void ClearAll();
	void AddItem(int row, int colnum, char * pStr);
	CLogViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLogViewerDlg)
	enum { IDD = IDD_LOGVIEWER_DIALOG };
	CComboBox	m_ctrlComboMonth2;
	CComboBox	m_ctrlComboDay2;
	CListCtrl	m_ctrlListText;
	CComboBox	m_ctrlComboDay;
	CComboBox	m_ctrlComboMonth;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLogViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnOpen();
	afx_msg void OnSelchangeComboMonth();
	afx_msg void OnSelchangeComboDay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListText(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGVIEWERDLG_H__32D0C901_BF8B_42F6_8F96_EB8566DDC527__INCLUDED_)
