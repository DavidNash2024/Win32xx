/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"

using namespace std;

/////////////////////////////////////
// CButtonDialog function definitions
//

// Constructor.
CButtonDialog::CButtonDialog(UINT resID) : CDialog(resID)
{
    m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

// Process the dialog's window messages.
INT_PTR CButtonDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_CTLCOLORDLG:    return OnCtlColorDlg(msg, wparam, lparam);
        case WM_CTLCOLORSTATIC: return OnCtlColorStatic(msg, wparam, lparam);
        }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Process the dialog's command messages (WM_COMMAND).
BOOL CButtonDialog::OnCommand(WPARAM wparam, LPARAM)
{
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

// Called when the button is pressed.
BOOL CButtonDialog::OnButton()
{
    TRACE("Push Button Pressed\n");
    return TRUE;
}

// Called when the check box 1 is clicked.
BOOL CButtonDialog::OnCheck1()
{
    TRACE("Check Box 1\n");
    return TRUE;
}

// Called when the check box 2 is clicked.
BOOL CButtonDialog::OnCheck2()
{
    TRACE("Check Box 2\n");
    return TRUE;
}

// Called when the check box 3 is clicked.
BOOL CButtonDialog::OnCheck3()
{
    TRACE("Check Box 3\n");
    return TRUE;
}

// Set the background color of the dialog
INT_PTR CButtonDialog::OnCtlColorDlg(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (IsXPThemed())
        return reinterpret_cast<INT_PTR>(m_brush.GetHandle());
    else
        return FinalWindowProc(msg, wparam, lparam);
}

// Set the background color of static controls
INT_PTR CButtonDialog::OnCtlColorStatic(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (IsXPThemed())
        return reinterpret_cast<INT_PTR>(m_brush.GetHandle());
    else
        return FinalWindowProc(msg, wparam, lparam);
}

// Called when a radio button is selected.
BOOL CButtonDialog::OnRangeOfRadioIDs(UINT firstID, UINT lastID, UINT clickedID)
{
    CheckRadioButton(firstID, lastID, clickedID);

    CString str;
    int button = clickedID - firstID + 1;
    str.Format(L"Radio%d", button);
    TRACE(str); TRACE("\n");

    return  TRUE;
}


///////////////////////////////////////
// CComboBoxDialog function definitions
//

// Constructor.
CComboBoxDialog::CComboBoxDialog(int resID) : CDialog(resID)
{
    m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

// Process the dialog's window messages.
INT_PTR CComboBoxDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
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

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Called before the dialog is displayed.
BOOL CComboBoxDialog::OnInitDialog()
{
    // Put some text in the Combo Boxes
    for (int i = 0 ; i < 6 ; i++)
    {
        SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"C Box 1"));
        SendDlgItemMessage(IDC_COMBO2, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"C Box 2"));
        SendDlgItemMessage(IDC_COMBO3, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"C Box 3"));
    }

    return TRUE;
}

/////////////////////////////////
// CMyDialog function definitions
//

// Constructor.
CMyDialog::CMyDialog(int resID) : CDialog(resID)
{
    m_pButtonDlg = nullptr;
    m_pComboDlg = nullptr;
}

// Process the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //    switch (msg)
    //    {
    //    }

        // Pass unhandled messages on to parent DialogProc
        return DialogProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Called when the dialog window is destroyed.
void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    AttachItem(IDC_TAB1, m_tab);

    m_pButtonDlg = static_cast<CButtonDialog*>(m_tab.AddTabPage(make_unique<CButtonDialog>(IDD_BUTTONS), L"Button Dialog"));
    m_pComboDlg = static_cast<CComboBoxDialog*>(m_tab.AddTabPage(make_unique<CComboBoxDialog>(IDD_COMBOBOXES), L"ComboBox Dialog"));
    m_tab.SelectPage(0);

    // Add some checkmarks to buttons to the button dialog
    m_pButtonDlg->CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
    m_pButtonDlg->CheckDlgButton(IDC_CHECK1, BST_CHECKED);

    return TRUE;
}

// This is called when the Enter key is pressed
void CMyDialog::OnOK()
{
    // Do default action (i.e. close the dialog)
    CDialog::OnOK();
}

