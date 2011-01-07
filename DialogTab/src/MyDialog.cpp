///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


/////////////////////////////////////////////
// Definitions for the CButtonDialog class
//
CButtonDialog::CButtonDialog(UINT nResID, HWND hWnd) : CDialog(nResID, hWnd) 
{
	m_hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
}

CButtonDialog::~CButtonDialog() 
{
	::DeleteObject(m_hBrush);
}

INT_PTR CButtonDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Set the background color of the dialog
	case WM_CTLCOLORDLG:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
	// Set the background color of static controls
	case WM_CTLCOLORSTATIC:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
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
		TRACE(_T("Push Button Pressed\n"));
		return TRUE;
	case IDC_RADIO1:
		TRACE(_T("Radio Button 1\n"));
		return TRUE;
	case IDC_RADIO2:
		TRACE(_T("Radio Button 2\n"));
		return TRUE;
	case IDC_RADIO3:
		TRACE(_T("Radio Button 3\n"));
		return TRUE;
	case IDC_CHECK1:
		TRACE(_T("Check Box 1\n"));
		return TRUE;
	case IDC_CHECK2:
		TRACE(_T("Check Box 2\n"));
		return TRUE;
	case IDC_CHECK3:
		TRACE(_T("Check Box 3\n"));
		return TRUE;
    } //switch (LOWORD(wParam))

	return FALSE;
}

/////////////////////////////////////////////
// Definitions for the CComboBoxDialog class
//
CComboBoxDialog::CComboBoxDialog(UINT nResID, HWND hWnd) : CDialog(nResID, hWnd) 
{
	m_hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
}

CComboBoxDialog::~CComboBoxDialog() 
{
	::DeleteObject(m_hBrush);
}

INT_PTR CComboBoxDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Set the background color of the dialog
	case WM_CTLCOLORDLG:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
	// Set the background color of static controls
	case WM_CTLCOLORSTATIC:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
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
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent)
{
}

CMyDialog::~CMyDialog()
{
}

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
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

