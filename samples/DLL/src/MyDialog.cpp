///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::OnDestroy()
{
    TRACE("Dialog inside DLL destroyed\n");
    ::PostQuitMessage(0);
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {

//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CMyDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTON1:   return OnButton();
    case IDC_CHECK1:    return OnCheck1();
    case IDC_CHECK2:    return OnCheck2();
    case IDC_CHECK3:    return OnCheck3();

    case IDC_RADIO1:    // intentionally blank
    case IDC_RADIO2:
    case IDC_RADIO3:    return OnRangeOfRadioIDs(IDC_RADIO1, IDC_RADIO3, id);
    }

    return FALSE;
}

BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDI_DIALOG);
    SetIconSmall(IDI_DIALOG);

    AttachItem(IDC_EDIT1, m_edit);
    AttachItem(IDC_RICHEDIT1, m_richEdit);

    // Put some text in the edit boxes
    m_edit.SetWindowText(_T("Edit Control"));
    m_richEdit.SetWindowText(_T("Rich Edit Window"));

    // Put some text in the list box
    for (int i = 0 ; i < 8 ; i++)
        SendDlgItemMessage(IDC_LIST1, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("List Box")));

    return TRUE;
}

void CMyDialog::OnOK()
{
    ::MessageBox(NULL, _T("OK Button Pressed.  Dialog will now end."), _T("Button"), MB_OK);
    CDialog::OnOK();
}

BOOL CMyDialog::OnButton()
{
    SetDlgItemText(IDC_STATIC3, _T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

BOOL CMyDialog::OnCheck1()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 1"));
    TRACE("Check Box 1\n");
    return TRUE;
}

BOOL CMyDialog::OnCheck2()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 2"));
    TRACE("Check Box 2\n");
    return TRUE;
}

BOOL CMyDialog::OnCheck3()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 3"));
    TRACE("Check Box 3\n");
    return TRUE;
}

BOOL CMyDialog::OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);

    CString str;
    int nButton = idClicked - idFirst + 1;
    str.Format(_T("Radio%d"), nButton);
    SetDlgItemText(IDC_STATIC3, str);
    TRACE(str); TRACE("\n");

    return TRUE;
}
