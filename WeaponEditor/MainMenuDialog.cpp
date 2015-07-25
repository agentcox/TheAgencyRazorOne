// MainMenuDialog.cpp : implementation file
//

#include "stdafx.h"
#include "weaponeditor.h"
#include "MainMenuDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MainMenuDialog dialog


MainMenuDialog::MainMenuDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MainMenuDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MainMenuDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MainMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainMenuDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MainMenuDialog, CDialog)
	//{{AFX_MSG_MAP(MainMenuDialog)
	ON_BN_CLICKED(CHOOSEAMMOED_BUTTON, OnButton)
	ON_BN_CLICKED(CHOOSEWEAPONED_BUTTON, OnButtonWeapon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainMenuDialog message handlers

void MainMenuDialog::OnButton() 
{

		// TODO: Add your control notification handler code here
	
}

void MainMenuDialog::OnButtonWeapon() 
{
	// TODO: Add your control notification handler code here
	
}
