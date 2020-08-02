///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


/////////////////////////////////////////////
// Definitions for the CButtonDialog class
//
CButtonDialog::CButtonDialog(UINT resID) : CDialog(resID)
{
    m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

CButtonDialog::~CButtonDialog()
{
}

INT_PTR CButtonDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CTLCOLORDLG:    return OnCtlColorDlg(msg, wparam, lparam);
    case WM_CTLCOLORSTATIC: return OnCtlColorStatic(msg, wparam, lparam);
    }
    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CButtonDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTON1:   return OnButton();
    case IDC_CHECK1:    return OnCheck1();
    case IDC_CHECK2:    return OnCheck2();
    case IDC_CHECK3:    return OnCheck3();

    case IDC_RADIO1:
    case IDC_RADIO2:    // intentionally blank
    case IDC_RADIO3:    return OnRangeOfRadioIDs(IDC_RADIO1, IDC_RADIO3, id);
    }

    return FALSE;
}

BOOL CButtonDialog::OnButton()
{
    TRACE("Push Button Pressed\n");
    return TRUE;
}

BOOL CButtonDialog::OnCheck1()
{
    TRACE("Check Box 1\n");
    return TRUE;
}

BOOL CButtonDialog::OnCheck2()
{
    TRACE("Check Box 2\n");
    return TRUE;
}

BOOL CButtonDialog::OnCheck3()
{
    TRACE("Check Box 3\n");
    return TRUE;
}

INT_PTR CButtonDialog::OnCtlColorDlg(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the background color of the dialog
    if (IsXPThemed())
        return reinterpret_cast<INT_PTR>(m_brush.GetHandle());
    else
        return FinalWindowProc(msg, wparam, lparam);
}

INT_PTR CButtonDialog::OnCtlColorStatic(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the background color of static controls
    if (IsXPThemed())
        return reinterpret_cast<INT_PTR>(m_brush.GetHandle());
    else
        return FinalWindowProc(msg, wparam, lparam);
}

BOOL CButtonDialog::OnRangeOfRadioIDs(UINT firstID, UINT lastID, UINT clickedID)
{
    CheckRadioButton(firstID, lastID, clickedID);

    CString str;
    int button = clickedID - firstID + 1;
    str.Format(_T("Radio%d"), button);
    TRACE(str); TRACE("\n");

    return  TRUE;
}


/////////////////////////////////////////////
// Definitions for the CComboBoxDialog class
//
CComboBoxDialog::CComboBoxDialog(UINT resID) : CDialog(resID)
{
    m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

CComboBoxDialog::~CComboBoxDialog()
{
}

INT_PTR CComboBoxDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    // Set the background color of the dialog
    case WM_CTLCOLORDLG:
        if (IsXPThemed()) return reinterpret_cast<INT_PTR>(m_brush.GetHandle());
        break;

    // Set the background color of static controls
    case WM_CTLCOLORSTATIC:
        if (IsXPThemed()) return reinterpret_cast<INT_PTR>(m_brush.GetHandle());
        break;

    }
    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CComboBoxDialog::OnInitDialog()
{
    // Put some text in the Combo Boxes
    for (int i = 0 ; i < 6 ; i++)
    {
        SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("C Box 1")));
        SendDlgItemMessage(IDC_COMBO2, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("C Box 2")));
        SendDlgItemMessage(IDC_COMBO3, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("C Box 3")));
    }

    return TRUE;
}

/////////////////////////////////////////
// Definitions for the CMyDialog class
//
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
    m_pButtonDlg = NULL;
    m_pComboDlg = NULL;
}

CMyDialog::~CMyDialog()
{
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//
//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
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

    AttachItem(IDC_TAB1, m_tab);

    m_pButtonDlg = static_cast<CButtonDialog*>(m_tab.AddTabPage(new CButtonDialog(IDD_BUTTONS), _T("Button Dialog")));
    m_pComboDlg = static_cast<CComboBoxDialog*>(m_tab.AddTabPage(new CComboBoxDialog(IDD_COMBOBOXES), _T("ComboBox Dialog")));
    m_tab.SelectPage(0);

    // Add some checkmarks to buttons to the button dialog
    m_pButtonDlg->CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
    m_pButtonDlg->CheckDlgButton(IDC_CHECK1, BST_CHECKED);

    return TRUE;
}

void CMyDialog::OnOK()
{
    // This is called when the Enter key is pressed

    // Do default action (i.e. close the dialog)
    CDialog::OnOK();
}

