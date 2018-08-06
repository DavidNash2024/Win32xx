///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CViewDialog class
CViewDialog::CViewDialog(UINT resID) : CDialog(resID)
{
}

CViewDialog::~CViewDialog()
{
}

void CViewDialog::AppendText(int id, LPCTSTR text)
{
    // This function appends text to an edit control

    // Append Line Feed
    LRESULT ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {
        SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
        SendDlgItemMessage(id, EM_REPLACESEL, 0, (LPARAM)(_T("\r\n")));
    }

    // Append text
    ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
    SendDlgItemMessage(id, EM_REPLACESEL, 0, (LPARAM)text);
}

INT_PTR CViewDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Pass resizing messages on to the resizer
    m_resizer.HandleMessage(msg, wparam, lparam);

//  switch (msg)
//  {
        //Additional messages to be handled go here
//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CViewDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTON1:   OnButton();     return TRUE;
    case IDC_CHECK1:    OnCheck1();     return TRUE;
    case IDC_CHECK2:    OnCheck2();     return TRUE;
    case IDC_CHECK3:    OnCheck3();     return TRUE;

    case IDC_RADIO1:    // intentionally blank
    case IDC_RADIO2:
    case IDC_RADIO3:    OnRangeOfRadioIDs(IDC_RADIO1, IDC_RADIO3, id); return TRUE;
    }

    return FALSE;
}

void CViewDialog::OnCancel()
{
    // Suppress default handling of OnCancel.
    
    AppendText(IDC_RICHEDIT2, _T("OnCancel called"));
    TRACE("OnCancel called \n");
}

void CViewDialog::OnClose()
{
    // Suppress the handling of WM_CLOSE.
    // Edit controls send a WM_CLOSE to the dialog when an Esc button is pressed.
}
BOOL CViewDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach CWnd objects to the dialog items
    AttachItem(IDC_BUTTON1, m_button);
    AttachItem(IDC_CHECK1,  m_checkA);
    AttachItem(IDC_CHECK2,  m_checkB);
    AttachItem(IDC_CHECK3,  m_checkC);
    AttachItem(IDC_RADIO1,  m_radioA);
    AttachItem(IDC_RADIO2,  m_radioB);
    AttachItem(IDC_RADIO3,  m_radioC);
    AttachItem(IDC_RICHEDIT1, m_richEdit1);
    AttachItem(IDC_RICHEDIT2, m_richEdit2);

    // Put some text in the edit boxes
    m_richEdit1.SetWindowText(_T("Rich Edit Window"));
    m_richEdit2.SetWindowText(_T("Rich Edit Window"));

    // Initialize dialog resizing
    m_resizer.Initialize( *this, CRect(0, 0, 300, 200) ); 
    m_resizer.AddChild(m_radioA,   topleft, 0);
    m_resizer.AddChild(m_radioB,   topleft, 0);
    m_resizer.AddChild(m_radioC,   topleft, 0);
    m_resizer.AddChild(m_button,   topleft, 0);
    m_resizer.AddChild(m_checkA,   bottomright, 0);
    m_resizer.AddChild(m_checkB,   bottomright, 0);
    m_resizer.AddChild(m_checkC,   bottomright, 0);
    m_resizer.AddChild(m_richEdit1, topright, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_richEdit2, bottomleft, RD_STRETCH_WIDTH| RD_STRETCH_HEIGHT);
    
    return TRUE;
}

void CViewDialog::OnOK()
{
    // Suppress default handling of OnOK.

    AppendText(IDC_RICHEDIT2, _T("OnOK called"));
    TRACE("OnOK called \n");
}

void CViewDialog::OnButton()
{
    AppendText(IDC_RICHEDIT2, _T("Button Pressed"));
    TRACE("Button Pressed\n");
}

void CViewDialog::OnCheck1()
{
    AppendText(IDC_RICHEDIT2, _T("Check Box 1"));
    TRACE("Check Box 1\n");
}

void CViewDialog::OnCheck2()
{
    AppendText(IDC_RICHEDIT2, _T("Check Box 2"));
    TRACE("Check Box 2\n");
}

void CViewDialog::OnCheck3()
{
    AppendText(IDC_RICHEDIT2, _T("Check Box 3"));
    TRACE("Check Box 3\n");
}

void CViewDialog::OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);

    CString str;
    int nButton = idClicked - idFirst + 1;
    str.Format(_T("Radio%d"), nButton);
    AppendText(IDC_RICHEDIT2, str);
    TRACE(str); TRACE("\n");
}
