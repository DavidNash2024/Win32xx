/////////////////////////////
// MyPropertySheet.cpp
//

#include "stdafx.h"
#include "MyPropertySheet.h"
#include "resource.h"

///////////////////////////////////
// CButtonPage function definitions
//

// Constructor.
CButtonPage::CButtonPage(UINT templateID, LPCTSTR title /* = NULL*/) : CPropertyPage(templateID, title)
{
}

// Proces the property sheet's window messages.
INT_PTR CButtonPage::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
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

    return DialogProcDefault(msg, wparam, lparam);
}

// Respond to the apply button.
BOOL CButtonPage::OnApply()
{
    TRACE ("Appy button pressed\n");

    SetModified(FALSE);
    return Validate();
}

// Respond to the cancel button.
void CButtonPage::OnCancel()
{
    TRACE ("Cancel button pressed\n");

    // Close the propertysheet.
    CPropertyPage::OnCancel();
}

// Called before the property sheet is displayed.
BOOL CButtonPage::OnInitDialog()
{
    // Center the PropertySheet (parent) over the frame before it is displayed.
    GetParent().CenterWindow();

    TRACE ("Button page created\n");
    return TRUE;
}

// Process the OK button.
void CButtonPage::OnOK()
{
    TRACE ("OK button pressed\n");

    // Close the propertysheet
    CPropertyPage::OnOK();
}

// Process the query cancel request.
BOOL CButtonPage::OnQueryCancel()
{
    TRACE ("Ok to Cancel?\n");

    return FALSE;    // Allow cancel to proceed
}

// Called when the propertysheet is activated.
BOOL CButtonPage::OnSetActive()
{
    TRACE("Button page is now active\n");

    // Set the wizard buttons
    SetWizardButtons(PSWIZB_NEXT);

    return TRUE;
}

// Validate the property sheet.
BOOL CButtonPage::Validate()
{
    // This is where we validate (and save) the contents of this page before it is closed
    // Here we would extract and test the values from the ButtonPage dialog:

    BOOL isValid = TRUE;

// Tell the user what went wrong
//  if (IsValid)
//      TRACE("Button Page Validation passed\n");
//  else
//      MessageBox(_T("Button Page Validation Failed"), _T("PageSheet Check"), MB_OK);

    return isValid;
}

//////////////////////////////////
// CComboPage function definitions
//

// Constructor.
CComboPage::CComboPage(UINT templateID, LPCTSTR title /* = NULL*/) : CPropertyPage(templateID, title)
{
}

// Process the property sheet's window messages.
INT_PTR CComboPage::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{

    switch (msg)
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

    return DialogProcDefault(msg, wparam, lparam);
}

// Called before the property sheet is displayed.
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

// Called when the property sheet is activated.
BOOL CComboPage::OnSetActive()
{
    TRACE("Combo page is now active\n");

    // Set the wizard buttons
    SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

    return TRUE;
}

////////////////////////////////////////
// CMyPropertySheet function definitions
//

// Constructor.
CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption /*=NULL*/, HWND hParent /* = NULL*/) : CPropertySheet(pszCaption, hParent)
{
    SetIcon(IDI_DIALOG);
}

// Called before the property sheet is displayed.
void CMyPropertySheet::OnInitialUpdate()
{
    // Remove system menu for wizards
    if (IsWizard())
    {
        DWORD style = GetStyle();
        style &= ~WS_SYSMENU;
        SetStyle(style);
    }
}

// Process the property sheet's window messages.
LRESULT CMyPropertySheet::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

