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
CViewDialog::CViewDialog(int resID) : CDialog(resID)
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
    try
    {
        // Pass resizing messages on to the resizer.
        m_Resizer.HandleMessage(msg, wparam, lparam);

        switch (msg)
        {
        case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
        }

        // Pass unhandled messages on to parent DialogProc.
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
    // WINDOWPLACEMENT maintains valid window position info when moving the frame
    // window between screens with different DPI scaling.
    WINDOWPLACEMENT wp;
    ZeroMemory(&wp, sizeof(wp));
    wp.length = sizeof(wp);
    m_picture.GetWindowPlacement(wp);
    int width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + width * 3 / 2;
    m_picture.SetWindowPlacement(wp);

    CRect rc = m_year.GetWindowRect();
    MapWindowPoints(*this, rc);

    // Initialize dialog resizing
    m_Resizer.Initialize(*this, CRect(0, 0, width + rc.right + 10, 210));

    m_Resizer.AddChild(m_static1, CResizer::topleft, 0);
    m_Resizer.AddChild(m_static2, CResizer::topleft, 0);
    m_Resizer.AddChild(m_static3, CResizer::topleft, 0);
    m_Resizer.AddChild(m_picture, CResizer::topright, 0);
    m_Resizer.AddChild(m_title,   CResizer::topright, RD_STRETCH_WIDTH);
    m_Resizer.AddChild(m_year,    CResizer::topleft, 0);
    m_Resizer.AddChild(m_actors,  CResizer::topleft, RD_STRETCH_WIDTH);
    m_Resizer.AddChild(m_info,    CResizer::topleft, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);

    // Set the rich edit controls to read only.
    m_title.SetReadOnly(TRUE);
    m_year.SetReadOnly(TRUE);
    m_actors.SetReadOnly(TRUE);
    m_info.SetReadOnly(TRUE);

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

