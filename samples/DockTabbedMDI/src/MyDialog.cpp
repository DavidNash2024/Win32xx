/////////////////////////////
// CViewDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


///////////////////////////////////
// CViewDialog function definitions
//

// Constructor.
CViewDialog::CViewDialog(UINT resID) : CDialog(resID)
{
}

// Destructor.
CViewDialog::~CViewDialog()
{
}

// This function appends text to an edit control.
void CViewDialog::AppendText(int id, LPCTSTR text)
{
    // Append Line Feed
    LRESULT dx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    if (dx)
    {
        SendDlgItemMessage(id, EM_SETSEL, dx, dx);
        SendDlgItemMessage(id, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(_T("\r\n")));
    }

    // Append text
    dx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(id, EM_SETSEL, dx, dx);
    SendDlgItemMessage(id, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(text));
}

// Process the dialog's window messages.
INT_PTR CViewDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass resizing messages on to the resizer
        m_resizer.HandleMessage(msg, wparam, lparam);

        switch (msg)
        {
        case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
        }

        // Pass unhandled messages on to parent DialogProc
        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Suppress closing the dialog when the Esc key is pressed.
void CViewDialog::OnCancel()
{
    AppendText(IDC_RICHEDIT2, _T("Cancel Pressed"));
    TRACE("Cancel Pressed\n");
}

// Suppress closing the dialog when an Esc key is pressed in the RichEdit controls.
void CViewDialog::OnClose()
{
}

// Process the command messages (WM_COMMAND) sent by the dialog's controls.
BOOL CViewDialog::OnCommand(WPARAM wparam, LPARAM)
{
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

// Called before the dialog is displayed.
BOOL CViewDialog::OnInitDialog()
{
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
    m_resizer.AddChild(m_radioA,    CResizer::topleft, 0);
    m_resizer.AddChild(m_radioB,    CResizer::topleft, 0);
    m_resizer.AddChild(m_radioC,    CResizer::topleft, 0);
    m_resizer.AddChild(m_button,    CResizer::topleft, 0);
    m_resizer.AddChild(m_checkA,    CResizer::bottomright, 0);
    m_resizer.AddChild(m_checkB,    CResizer::bottomright, 0);
    m_resizer.AddChild(m_checkC,    CResizer::bottomright, 0);
    m_resizer.AddChild(m_richEdit1, CResizer::topright, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_richEdit2, CResizer::bottomleft, RD_STRETCH_WIDTH| RD_STRETCH_HEIGHT);

    // Select the first radio button.
    CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);

    return TRUE;
}

// Respond to a mouse click on the dialog window.
LRESULT CViewDialog::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!IsChild(::GetFocus()))
        SetFocus();

    return FinalWindowProc(msg, wparam, lparam);
}

// Suppress closing the dialog when the enter key is pressed.
void CViewDialog::OnOK()
{
    AppendText(IDC_RICHEDIT2, _T("OK Button Pressed"));
    TRACE("OK Button Pressed\n");
}

// Called when the button is pressed.
BOOL CViewDialog::OnButton()
{
    AppendText(IDC_RICHEDIT2, _T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

// Called when check box 1 is clicked.
BOOL CViewDialog::OnCheck1()
{
    AppendText(IDC_RICHEDIT2, _T("Check Box 1"));
    TRACE("Check Box 1\n");
    return TRUE;
}

// Called when check box 2 is clicked.
BOOL CViewDialog::OnCheck2()
{
    AppendText(IDC_RICHEDIT2, _T("Check Box 2"));
    TRACE("Check Box 2\n");
    return TRUE;
}

// Called when check box 3 is clicked.
BOOL CViewDialog::OnCheck3()
{
    AppendText(IDC_RICHEDIT2, _T("Check Box 3"));
    TRACE("Check Box 3\n");
    return TRUE;
}

// Called when a radio button is selected.
BOOL CViewDialog::OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);

    CString str;
    int button = idClicked - idFirst + 1;
    str.Format(_T("Radio%d"), button);
    AppendText(IDC_RICHEDIT2, str);
    TRACE(str); TRACE("\n");

    return TRUE;
}


//////////////////////////////////////
// CContainDialog function definitions
//

// Constructor.
CContainDialog::CContainDialog() : m_viewDialog(IDD_MYDIALOG)
{
    SetView(m_viewDialog);
    SetDockCaption (_T("Dialog View - Docking container"));
    SetTabText(_T("Dialog"));
    SetTabIcon(IDI_DIALOGVIEW);
}


///////////////////////////////////
// CDockDialog function definitions
//

// Constructor.
CDockDialog::CDockDialog()
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

