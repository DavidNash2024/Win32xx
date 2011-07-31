///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
}
/*
CMyDialog::CMyDialog(LPCTSTR lpszResName, CWnd* pParent)
	: CDialog(lpszResName, pParent)
{
}
*/
CMyDialog::~CMyDialog()
{
}

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass resizing messages on to the resizer
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

//	switch (uMsg)
//	{
//		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam); 
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Attach the custom control to a CWnd object
	m_MyControl.AttachDlgItem(IDC_CUSTOMWEB, this);

	// Support automatic resizing
	m_Resizer.Initialize( this, CRect(0, 0, 300, 200) );
	m_Resizer.AddChild(m_MyControl, topleft, RD_STRETCH_WIDTH|RD_STRETCH_HEIGHT);
	
	return TRUE;
}

