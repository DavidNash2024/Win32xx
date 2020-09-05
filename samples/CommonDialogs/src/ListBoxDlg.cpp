/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]           (ListBoxDlg.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CListBoxDlg popup
    dialog class for applications using the Win32++ Windows interface
    classes.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "ListBoxDlg.h"
#include "resource.h"

/*============================================================================*/
    CListBoxDlg::
CListBoxDlg()                                                               /*

*-----------------------------------------------------------------------------*/
    :  CDialog(IDD_CLISTVIEWBOX)
{
    SetBoxTitle(_T("List"));
    AddListItem(_T("default item 0"));
}

/*============================================================================*/
    INT_PTR CListBoxDlg::
DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)                         /*

    The list box message loop.
*-----------------------------------------------------------------------------*/
{
    switch (msg)
    {
        case WM_COMMAND:
        {
            switch (HIWORD(wparam))
            {
                case LBN_DBLCLK:
                {     // select the current color
                    OnOK();
                    break;
                }
            }
        }
        case WM_SYSCOMMAND:
        {
            switch (wparam)
            {     // handle closing the dialog via [X]
                case SC_CLOSE:
                    OnCancel();
                    return TRUE;
            }
        }
    }
      // pass unhandled messages on for default processing
    return DialogProcDefault(msg, wparam, lparam);
}

/*============================================================================*/
    void CListBoxDlg::
OnOK()                                                                      /*

    This method executes after the box terminates when the OK button was
    activated.
*-----------------------------------------------------------------------------*/
{
    int choice = m_listBox.GetCurSel();
      // return the choice: see the note above
    CDialog::EndDialog(choice >= 0 ? choice : INT_MAX);
}

/*============================================================================*/
    void CListBoxDlg::
OnCancel()                                                                  /*

    This method executes after the box terminates and either Cancel or [X]
    button on the dialog caption was activated. Since the CDialog::() method
    returns the parameter sent in the EndDialog() statement, this member
    sets that value to INT_MAX so that no unwanted assert(0) occurs.
*-----------------------------------------------------------------------------*/
{
    CDialog::EndDialog(INT_MAX);
}

/*============================================================================*/
    BOOL  CListBoxDlg::
OnInitDialog()                                                              /*

    Set the dialog banner text and list box contents to values prestored
    in the list box string list. Locate the list box in the center of the
    window.
*-----------------------------------------------------------------------------*/
{
    SetWindowTitle();
      // subclass the dialog controls
    m_listBox.AttachDlgItem(IDC_LIST_BOX, *this);
    m_listBox.ResetContent();
    for (UINT i = 0; i < m_listContent.size(); i++)
        m_listBox.AddString(m_listContent[i]);
    return TRUE;
}
/*----------------------------------------------------------------------------*/
