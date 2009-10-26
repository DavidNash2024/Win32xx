//////////////////////////////////////////////
// MyPropertySheet.cpp

#include "stdafx.h"
#include "Mainfrm.h"
#include "MyPropertySheet.h"
#include "resource.h"


CButtonPage::CButtonPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

BOOL CButtonPage::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		PropSheet_Changed(GetParent(), m_hWnd);
		break;

	default:
		break;
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

void CButtonPage::OnApply()
{ 
	TRACE (_T("Appy button pressed\n")); 
	
	// Call the base class's OnApply
	CPropertyPage::OnApply(); 
}

void CButtonPage::OnCancel()
{ 
	TRACE (_T("Cancel button pressed\n")); 
}

BOOL CButtonPage::OnInitDialog()
{ 
	TRACE (_T("Button page created\n")); 
	return TRUE; 
}

void CButtonPage::OnOK()
{ 
	TRACE (_T("OK button pressed\n"));

	// Call the base class's OnOK
	CPropertyPage::OnOK(); 
}

BOOL CButtonPage::OnQueryCancel()
{ 
	TRACE (_T("Ok to Cancel?\n")); 
	
	// return TRUE means OK to cancel
	return TRUE; 
}

void CButtonPage::OnSetActive()
{
	TRACE("Button page is now active\n");

	// Set the wizard buttons
	PropSheet_SetWizButtons(GetParent(), PSWIZB_NEXT);
}

int CButtonPage::Validate()
{
	// This is where we validate (and save) the contents of this page before it is closed
	
	// return one of these values:
	// PSNRET_NOERROR. The changes made to this page are valid and have been applied
	// PSNRET_INVALID. The property sheet will not be destroyed, and focus will be returned to this page.
	// PSNRET_INVALID_NOCHANGEPAGE. The property sheet will not be destroyed, and focus will be returned 
	//                               to the page that had focus when the button was pressed.


	int nStatus = PSNRET_NOERROR;
//	int nStatus = PSNRET_INVALID;
//	int nStatus = PSNRET_INVALID_NOCHANGEPAGE;

// Tell the user what went wrong
	if (nStatus != PSNRET_NOERROR)
		MessageBox(_T("Button Page Validation Failed"), _T("PageSheet Check"), MB_OK);
	else
		TRACE(_T("Button Page Validation passed\n"));
	
	return nStatus;
}

CComboPage::CComboPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

BOOL CComboPage::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		PropSheet_Changed(GetParent(), m_hWnd);
		break;

	default:
		break;
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CComboPage::OnInitDialog()
{
	// Put some text in the Combo Boxes
	for (int i = 0 ; i < 6 ; i++)
	{
		::SendMessage( GetDlgItem(IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM) _T("C Box 1"));
		::SendMessage( GetDlgItem(IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM) _T("C Box 2"));
		::SendMessage( GetDlgItem(IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM) _T("C Box 3"));
	}

	return TRUE;
}

void CComboPage::OnSetActive()
{
	// Set the wizard buttons
	PropSheet_SetWizButtons(GetParent(), PSWIZB_BACK | PSWIZB_FINISH);
}


CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption /*=NULL*/, HWND hwndParent /* = NULL*/) : CPropertySheet(pszCaption, hwndParent)
{
	m_PSH.pszIcon          = MAKEINTRESOURCE(IDI_BACKCOLOR);
	m_PSH.dwFlags          = PSH_PROPSHEETPAGE | PSH_USEICONID  | PSH_USECALLBACK;
}

LRESULT CMyPropertySheet::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);	
}

