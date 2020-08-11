/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


/////////////////////////////////
// CMyDialog function definitions
//

// Constructor.
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

// Destructor.
CMyDialog::~CMyDialog()
{
}

// Process the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
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

// Process the dialog's command messages(WM_COMMAND)
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

    case IDC_RADIO1:
    case IDC_RADIO2:    // intentionally blank
    case IDC_RADIO3:    return OnRangeOfRadioIDs(IDC_RADIO1, IDC_RADIO3, id);
    }

    return FALSE;
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

// Called when the OK button or Enter key is pressed.
void CMyDialog::OnOK()
{
    MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
    CDialog::OnOK();
}

// Called when the button is pressed.
BOOL CMyDialog::OnButton()
{
    SetDlgItemText(IDC_STATIC3, _T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

// Called when check box 1 is clicked.
BOOL CMyDialog::OnCheck1()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 1"));
    TRACE("Check Box 1\n");
    return TRUE;
}

// Called when check box 2 is clicked.
BOOL CMyDialog::OnCheck2()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 2"));
    TRACE("Check Box 2\n");
    return TRUE;
}

// Called when check box 3 is clicked.
BOOL CMyDialog::OnCheck3()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 3"));
    TRACE("Check Box 3\n");
    return TRUE;
}

// Called when the dialog's background is redrawn.
BOOL CMyDialog::OnEraseBkgnd(CDC& dc)
{
    // Adding a gripper to a resizable dialog is a bit of a hack, but since it
    // is often done, here is one method of doing it safely.

    UNREFERENCED_PARAMETER(dc);

    // Draw the dialog's background manually
    CRect rc = GetClientRect();
    CClientDC dcClient(*this);
    dcClient.SolidFill(GetSysColor(COLOR_3DFACE), rc);

    // draw size grip
    if (rc.Width() > m_resizer.GetMinRect().Width() && rc.Height() > m_resizer.GetMinRect().Height())
    {
        int size = GetSystemMetrics(SM_CXVSCROLL);
        rc.left = rc.right - size;
        rc.top = rc.bottom - size;
        dcClient.DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
    }

    // Suppress default background drawing
    return TRUE;
}

// Called when a radio button is selected.
BOOL CMyDialog::OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);

    CString str;
    int nButton = idClicked - idFirst + 1;
    str.Format(_T("Radio%d"), nButton);
    TRACE(str); TRACE("\n");

    return TRUE;
}

