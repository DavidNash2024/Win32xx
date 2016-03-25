//////////////////////////////////////////////
// MyPropertySheet.cpp

#include "stdafx.h"
#include "PropertySheetApp.h"
#include "Mainfrm.h"
#include "MyPropertySheet.h"
#include "resource.h"


CButtonPage::CButtonPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

INT_PTR CButtonPage::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		{
			PropSheet_Changed(GetParent(), GetHwnd());
			break;
		}

	default:
		break;
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

int CButtonPage::OnApply()
{ 
	TRACE ("Appy button pressed\n"); 
	
	// The possible return values are:
	// PSNRET_NOERROR. The changes made to this page are valid and have been applied
	// PSNRET_INVALID. The property sheet will not be destroyed, and focus will be returned to this page.
	// PSNRET_INVALID_NOCHANGEPAGE. The property sheet will not be destroyed, and focus will be returned;

	return Validate();
}

void CButtonPage::OnCancel()
{ 
	TRACE ("Cancel button pressed\n");

	// Close the modeless propertysheet
	GetParent().PostMessage(WM_CLOSE);	
}

BOOL CButtonPage::OnInitDialog()
{ 
	TRACE ("Button page created\n"); 
	return TRUE; 
}

int CButtonPage::OnOK()
{ 
	TRACE ("OK button pressed\n");

	// The possible return values are:
	// PSNRET_NOERROR. The changes made to this page are valid and have been applied
	// PSNRET_INVALID. The property sheet will not be destroyed, and focus will be returned to this page.
	// PSNRET_INVALID_NOCHANGEPAGE. The property sheet will not be destroyed, and focus will be returned;

	// Close the modeless propertysheet
	GetParent().PostMessage(WM_CLOSE);
	
	return Validate(); 
}

BOOL CButtonPage::OnQueryCancel()
{ 
	TRACE ("Ok to Cancel?\n"); 
	
	return FALSE;    // Allow cancel to proceed
}

int CButtonPage::OnSetActive()
{
	TRACE("Button page is now active\n");

	// Set the wizard buttons
	SetWizardButtons(PSWIZB_NEXT);

	return 0;
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
		TRACE("Button Page Validation passed\n");
	
	return nStatus;
}

CComboPage::CComboPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

INT_PTR CComboPage::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		{
			PropSheet_Changed(GetParent(), GetHwnd());
			break;
		}

	default:
		break;
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CComboPage::OnInitDialog()
{
	TRACE("Combo page created\n");

	// Put some text in the Combo Boxes
	for (int i = 0 ; i < 6 ; i++)
	{
		SendDlgItemMessage( IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM) _T("C Box 1") );
		SendDlgItemMessage( IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM) _T("C Box 2") );
		SendDlgItemMessage( IDC_COMBO3, CB_ADDSTRING, 0, (LPARAM) _T("C Box 3") );
	}

	return TRUE;
}

int CComboPage::OnSetActive()
{
	TRACE("Combo page is now active\n");

	// Set the wizard buttons
	SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

	return 0;
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption /*=NULL*/, HWND hParent /* = NULL*/) : CPropertySheet(pszCaption, hParent)
{
	SetIcon(IDI_DIALOG);
}

void CMyPropertySheet::OnInitialUpdate()
{
	// Remove system menu for wizards
	if (IsWizard())
	{
		DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
		dwStyle &= ~WS_SYSMENU;
		SetWindowLongPtr(GWL_STYLE, dwStyle);
	}

	CenterWindow();
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

