///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


/////////////////////////////////////////////
// Definitions for the CButtonDialog class
//
CButtonDialog::CButtonDialog(UINT nResID) : CDialog(nResID) 
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

CButtonDialog::~CButtonDialog() 
{
}

INT_PTR CButtonDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CTLCOLORDLG:	return OnCtlColorDlg(uMsg, wParam, lParam);
	case WM_CTLCOLORSTATIC:	return OnCtlColorStatic(uMsg, wParam, lParam);	
	}
	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CButtonDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
    {
	case IDC_BUTTON1:
		TRACE("Push Button Pressed\n");
		return TRUE;
	case IDC_RADIO1:
		TRACE("Radio Button 1\n");
		return TRUE;
	case IDC_RADIO2:
		TRACE("Radio Button 2\n");
		return TRUE;
	case IDC_RADIO3:
		TRACE("Radio Button 3\n");
		return TRUE;
	case IDC_CHECK1:
		TRACE("Check Box 1\n");
		return TRUE;
	case IDC_CHECK2:
		TRACE("Check Box 2\n");
		return TRUE;
	case IDC_CHECK3:
		TRACE("Check Box 3\n");
		return TRUE;
    } //switch (LOWORD(wParam))

	return FALSE;
}

INT_PTR CButtonDialog::OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Set the background color of the dialog
	if (IsXPThemed()) 
		return (INT_PTR)m_Brush.GetHandle();
	else;
		return FinalWindowProc(uMsg, wParam, lParam);
}

INT_PTR CButtonDialog::OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Set the background color of static controls
	if (IsXPThemed()) 
		return (INT_PTR)m_Brush.GetHandle();
	else
		return FinalWindowProc(uMsg, wParam, lParam);
}


/////////////////////////////////////////////
// Definitions for the CComboBoxDialog class
//
CComboBoxDialog::CComboBoxDialog(UINT nResID) : CDialog(nResID) 
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

CComboBoxDialog::~CComboBoxDialog() 
{
}

INT_PTR CComboBoxDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Set the background color of the dialog
	case WM_CTLCOLORDLG:
		if (IsXPThemed()) return (INT_PTR)m_Brush.GetHandle();
		break;
	
	// Set the background color of static controls
	case WM_CTLCOLORSTATIC:
		if (IsXPThemed()) return (INT_PTR)m_Brush.GetHandle();
		break;
	
	}
	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CComboBoxDialog::OnInitDialog()
{
	// Put some text in the Combo Boxes
	for (int i = 0 ; i < 6 ; i++)
	{
		SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM) _T("C Box 1"));
		SendDlgItemMessage(IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM) _T("C Box 2"));
		SendDlgItemMessage(IDC_COMBO3, CB_ADDSTRING, 0, (LPARAM) _T("C Box 3"));
	}

	return TRUE;
}

/////////////////////////////////////////
// Definitions for the CMyDialog class
//
CMyDialog::CMyDialog(UINT nResID) : CDialog(nResID)
{
}

CMyDialog::~CMyDialog()
{
}

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

void CMyDialog::OnDestroy()
{
	// End the application
	::PostQuitMessage(0);
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);
	
	AttachItem(IDC_TAB1, m_Tab);
	m_Tab.AddTabPage(new CButtonDialog(IDD_BUTTONS), _T("Button Dialog"));
	m_Tab.AddTabPage(new CComboBoxDialog(IDD_COMBOBOXES), _T("ComboBox Dialog"));
	m_Tab.SelectPage(0);

	return true;
}

void CMyDialog::OnOK()
{
	// This is called when the Enter key is pressed

	// Do default action (i.e. close the dialog)
	CDialog::OnOK();
}

