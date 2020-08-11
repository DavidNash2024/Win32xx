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
void CViewDialog::AppendText(int nID, LPCTSTR text)
{
    // Append Line Feed.
    LRESULT ndx = SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {
        SendDlgItemMessage(nID, EM_SETSEL, ndx, ndx);
        SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM)(L"\r\n"));
    }

    // Append text
    ndx = SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(nID, EM_SETSEL, ndx, ndx);
    SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM)text);
}

// Process the dialog's window messages.
INT_PTR CViewDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Pass resizing messages on to the resizer.
    m_Resizer.HandleMessage(msg, wparam, lparam);

    switch (msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

// Called when the dialog is created, but before it is displayed.
BOOL CViewDialog::OnInitDialog()
{
    // Attach CWnd objects to the dialog items
    AttachItem(IDC_STATIC1, m_static1);
    AttachItem(IDC_STATIC2, m_static2);
    AttachItem(IDC_STATIC3, m_static3);
    AttachItem(IDC_PICTURE, m_picture);
    AttachItem(IDC_TITLE, m_title);
    AttachItem(IDC_YEAR, m_year);
    AttachItem(IDC_ACTORS, m_actors);
    AttachItem(IDC_DESCRIPTION, m_info);

    // Correct the cover image aspect ratio (custom scaling affects dialog co-ords).
    CRect rc = m_picture.GetClientRect();
    rc.bottom = rc.right * 3 / 2;
    m_picture.ClientToScreen(rc);
    MapWindowPoints(*this, rc);
    m_picture.SetWindowPos(NULL, rc, SWP_SHOWWINDOW);

    // Initialize dialog resizing
    m_Resizer.Initialize( *this, CRect(0, 0, 400, 200) );

    m_Resizer.AddChild(m_static1, topleft, 0);
    m_Resizer.AddChild(m_static2, topleft, 0);
    m_Resizer.AddChild(m_static3, topleft, 0);
    m_Resizer.AddChild(m_picture,topright, 0);
    m_Resizer.AddChild(m_title, topleft, RD_STRETCH_WIDTH);
    m_Resizer.AddChild(m_year, topleft, 0);
    m_Resizer.AddChild(m_actors, topleft, RD_STRETCH_WIDTH);
    m_Resizer.AddChild(m_info, bottomright, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);

    // Set the rich edit controls to read only.
    m_title.SetReadOnly(TRUE);
    m_year.SetReadOnly(TRUE);
    m_actors.SetReadOnly(TRUE);
    m_info.SetReadOnly(TRUE);

    // Set the fonts.
    NONCLIENTMETRICS info = GetNonClientMetrics();
    LOGFONT lf = info.lfMenuFont;
    StrCopy(lf.lfFaceName, L"Consolas", 32);    // A good font for text
    m_textFont.CreateFontIndirect(lf);

    m_year.SetFont(m_textFont, FALSE);
    m_actors.SetFont(m_textFont, FALSE);
    m_info.SetFont(m_textFont, FALSE);
    SetFont(m_textFont, FALSE);

    StrCopy(lf.lfFaceName, L"Tahoma", 32);  // A good font for titles
    lf.lfWeight = lf.lfWeight * 2;      // Bold
    m_titleFont.CreateFontIndirect(lf);
    m_title.SetFont(m_titleFont, FALSE);

    return TRUE;
}

// Called when the mouse is clicked on the dialog.
LRESULT CViewDialog::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Change focus to the dialog, unless a child already has focus.
    if (!IsChild(::GetFocus()))
        SetFocus();

    return FinalWindowProc(msg, wparam, lparam);
}


///////////////////////////////////
// CDockDialog function definitions
//

// Constructor.
CDockDialog::CDockDialog() : m_view(IDD_MYDIALOG)
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

