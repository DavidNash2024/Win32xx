/* (26-Mar-2025)                                              (ListBoxDlg.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CListBoxDlg popup
    dialog class for applications using the Win32++ Windows interface
    classes.

*******************************************************************************/

#include "stdafx.h"
#include "ListBoxDlg.h"
#include "resource.h"

CListBoxDlg::CListBoxDlg() : CDialog(IDD_CLISTVIEWBOX)
{
    SetBoxTitle(L"List");
    AddListItem(L"default item 0");
}

// The list box message loop.
INT_PTR CListBoxDlg::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_COMMAND:
        {
            switch (HIWORD(wparam))
            {
                case LBN_DBLCLK:
                {
                    // Select the current color.
                    OnOK();
                    break;
                }
            }
            break;
        }
        case WM_SYSCOMMAND:
        {
            switch (wparam)
            {
                case SC_CLOSE:
                    OnCancel();
                    return TRUE;
            }
        }
    }

    // Pass unhandled messages on for default processing.
    return DialogProcDefault(msg, wparam, lparam);
}

// This method executes after the box terminates when the OK button was
// activated.
void CListBoxDlg::OnOK()
{
    int choice = m_listBox.GetCurSel();
    CDialog::EndDialog(choice >= 0 ? choice : INT_MAX);
}

// This method executes after the box terminates and either Cancel or [X]
// button on the dialog caption was activated.
void CListBoxDlg::OnCancel()
{
    CDialog::EndDialog(INT_MAX);
}

// Set the dialog banner text and list box contents to values.
BOOL  CListBoxDlg::OnInitDialog()
{
    SetWindowText(m_boxTitle);

    // Subclass the dialog controls.
    m_listBox.AttachDlgItem(IDC_LIST_BOX, *this);
    m_listBox.ResetContent();
    for (UINT i = 0; i < m_listContent.size(); i++)
    {
        m_listBox.AddString(m_listContent[i]);
    }
    return TRUE;
}
