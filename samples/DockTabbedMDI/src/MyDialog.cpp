///////////////////////////////////////
// CViewDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CViewDialog class
CViewDialog::CViewDialog(UINT nResID) : CDialog(nResID)
{
	m_hInstRichEdit = LoadLibrary(_T("RICHED32.DLL"));
    if (m_hInstRichEdit == 0)
 		::MessageBox(NULL, _T("CViewDialog::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
}

CViewDialog::~CViewDialog()
{
	::FreeLibrary(m_hInstRichEdit);
}

INT_PTR CViewDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass resizing messages on to the resizer
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CViewDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
    {
	case IDC_BUTTON1:	OnButton();		return TRUE;
	case IDC_RADIO1:	OnRadio1();		return TRUE;
	case IDC_RADIO2:	OnRadio2();		return TRUE;
	case IDC_RADIO3:	OnRadio3();		return TRUE;
	case IDC_CHECK1:	OnCheck1();		return TRUE;
	case IDC_CHECK2:	OnCheck2();		return TRUE;
	case IDC_CHECK3:	OnCheck3();		return TRUE;
    }

	return FALSE;
}

BOOL CViewDialog::OnInitDialog()
{
	// Attach CWnd objects to the dialog items
	AttachItem(IDC_BUTTON1,	m_Button);
	AttachItem(IDC_CHECK1,	m_CheckA);
	AttachItem(IDC_CHECK2,	m_CheckB);
	AttachItem(IDC_CHECK3,	m_CheckC);
	AttachItem(IDC_RADIO1,	m_RadioA);
	AttachItem(IDC_RADIO2,	m_RadioB);
	AttachItem(IDC_RADIO3,	m_RadioC);
	AttachItem(IDC_RICHEDIT1, m_RichEdit1);
	AttachItem(IDC_RICHEDIT2, m_RichEdit2);

	// Put some text in the edit boxes
	m_RichEdit1.SetWindowText(_T("Rich Edit Window"));
	m_RichEdit2.SetWindowText(_T("Rich Edit Window"));

	// Initialize dialog resizing
	m_Resizer.Initialize( this, CRect(0, 0, 300, 200) ); 
	m_Resizer.AddChild(m_RadioA,   topleft, 0);
	m_Resizer.AddChild(m_RadioB,   topleft, 0);
	m_Resizer.AddChild(m_RadioC,   topleft, 0);
	m_Resizer.AddChild(m_Button,   topleft, 0);
	m_Resizer.AddChild(m_CheckA,   bottomright, 0);
	m_Resizer.AddChild(m_CheckB,   bottomright, 0);
	m_Resizer.AddChild(m_CheckC,   bottomright, 0);
	m_Resizer.AddChild(m_RichEdit1, topright, RD_STRETCH_WIDTH);
	m_Resizer.AddChild(m_RichEdit2, bottomleft, RD_STRETCH_WIDTH| RD_STRETCH_HEIGHT);
	
	return true;
}

void CViewDialog::OnOK()
{
	MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
	CDialog::OnOK();
}

void CViewDialog::OnButton()
{
	SetDlgItemText(IDC_STATIC3, _T("Button Pressed"));
	TRACE("Button Pressed\n");
}

void CViewDialog::OnCheck1()
{
	SetDlgItemText(IDC_STATIC3, _T("Check Box 1"));
	TRACE("Check Box 1\n");
}

void CViewDialog::OnCheck2()
{
	SetDlgItemText(IDC_STATIC3, _T("Check Box 2"));
	TRACE("Check Box 2\n");
}

void CViewDialog::OnCheck3()
{
	SetDlgItemText(IDC_STATIC3, _T("Check Box 3"));
	TRACE("Check Box 3\n");
}

void CViewDialog::OnRadio1()
{
	SetDlgItemText(IDC_STATIC3, _T("Radio 1"));
	TRACE("Radio 1\n");
}

void CViewDialog::OnRadio2()
{
	SetDlgItemText(IDC_STATIC3, _T("Radio 2"));
	TRACE("Radio 2\n");
}

void CViewDialog::OnRadio3()
{
	SetDlgItemText(IDC_STATIC3, _T("Radio 3"));
	TRACE("Radio 3\n");
}


//////////////////////////////////////////////
//  Definitions for the CContainDialog class
CContainDialog::CContainDialog() : m_ViewDialog(IDD_MYDIALOG)
{
	SetView(m_ViewDialog); 
	SetDockCaption (_T("Dialog View - Docking container"));
	SetTabText(_T("Dialog"));
	SetTabIcon(IDI_DIALOGVIEW);
} 


//////////////////////////////////////////////
//  Definitions for the CDockDialog class
CDockDialog::CDockDialog()
{
	// Set the view window to our edit control
	SetView(m_View);

	// Set the width of the splitter bar
	SetBarWidth(8);
}


