///////////////////////////////////////
// SearchDialog.cpp

#include "stdafx.h"
#include "SearchDialog.h"
#include "resource.h"


// Definitions for the CSearchDialog class
CSearchDialog::CSearchDialog(UINT resID) : CDialog(resID)
{
}

CSearchDialog::~CSearchDialog()
{
}

INT_PTR CSearchDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CSearchDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
    UINT nID = LOWORD(wParam);
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

