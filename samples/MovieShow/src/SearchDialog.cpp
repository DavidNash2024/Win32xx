/////////////////////////////
// SearchDialog.cpp
//

#include "stdafx.h"
#include "SearchDialog.h"
#include "resource.h"

//////////////////////////////////////
// CSearchDialog function definitions.
//

// Constructor.
CSearchDialog::CSearchDialog(UINT resID) : CDialog(resID)
{
}

// Destructor.
CSearchDialog::~CSearchDialog()
{
}

// Process the dialog's window messages.
INT_PTR CSearchDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
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

// Respond to command messages (WM_COMMAND) from the dialog's controls.
BOOL CSearchDialog::OnCommand(WPARAM wparam, LPARAM /*lparam*/)
{
    UINT nID = LOWORD(wparam);
    switch (nID)
    {
    case IDC_CHECKTITLE:
    {
        UINT state = m_titleButton.GetState();
        m_titleWindow.EnableWindow(state & BST_CHECKED);
        m_titleWindow.SetFocus();
    }
    break;
    case IDC_CHECKACTORS:
    {
        UINT state = m_actorsButton.GetState();
        m_actorsWindow.EnableWindow(state & BST_CHECKED);
        m_actorsWindow.SetFocus();

    }
    break;
    case IDC_CHECKINFO:
    {
        UINT state = m_infoButton.GetState();
        m_infoWindow.EnableWindow(state & BST_CHECKED);
        m_infoWindow.SetFocus();
    }
    break;
    }

    return FALSE;
}

// Called before the dialog is displayed.
BOOL CSearchDialog::OnInitDialog()
{
    // Attach CWnd objects to the dialog items
    AttachItem(IDC_CHECKTITLE, m_titleButton);
    AttachItem(IDC_CHECKACTORS, m_actorsButton);
    AttachItem(IDC_CHECKINFO, m_infoButton);

    AttachItem(IDC_EDITTITLE, m_titleWindow);
    AttachItem(IDC_EDITACTORS, m_actorsWindow);
    AttachItem(IDC_EDITINFO, m_infoWindow);

    m_titleWindow.EnableWindow(FALSE);
    m_actorsWindow.EnableWindow(FALSE);
    m_infoWindow.EnableWindow(FALSE);

    return TRUE;
}

// Called when the Enter key or OK button is pressed.
void CSearchDialog::OnOK()
{
    if (m_titleButton.GetState() & BST_CHECKED)
        m_titleText = m_titleWindow.GetWindowText();

    if (m_actorsButton.GetState() & BST_CHECKED)
        m_actorsText = m_actorsWindow.GetWindowText();

    if (m_infoButton.GetState() & BST_CHECKED)
        m_infoText = m_infoWindow.GetWindowText();

    CDialog::OnOK();
}

