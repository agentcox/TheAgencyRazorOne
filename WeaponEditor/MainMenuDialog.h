#if !defined(AFX_MAINMENUDIALOG_H__F91A26E3_F9D4_11D3_8437_0080C8FE513D__INCLUDED_)
#define AFX_MAINMENUDIALOG_H__F91A26E3_F9D4_11D3_8437_0080C8FE513D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainMenuDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MainMenuDialog dialog

class MainMenuDialog : public CDialog
{
// Construction
public:
	MainMenuDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MainMenuDialog)
	enum { IDD = MAINMENUDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainMenuDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MainMenuDialog)
	afx_msg void OnButton();
	afx_msg void OnButtonWeapon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINMENUDIALOG_H__F91A26E3_F9D4_11D3_8437_0080C8FE513D__INCLUDED_)
