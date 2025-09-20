/* (26-Mar-2025)                                                    (View.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CView class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes. This particular view class contains features for selection of
    client background color, selection of edit box font, and use of external
    serialization files.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

// Construct default window main view object.
CView::CView(UINT id) : CDialog(id), m_parent(nullptr)
{
}

// Add the string with the resource id to the control whose resource
// identifier is also id.
BOOL CView::AddToolTip(HWND parent, UINT id)
{
    return AddToolTip(parent, id, LoadString(id));
}

// Adds the toolTip string to the control whose resource identifier is id.
BOOL CView::AddToolTip(HWND parent, UINT id, LPCWSTR toolTip)
{
    HWND hCtl = ::GetDlgItem(parent, id);
    if (hCtl == nullptr)
    {
        TRACE(CString("cannot connect tooltip: ") + toolTip);
        return FALSE;
    }

    if (!m_toolTip.AddTool(hCtl, toolTip))
    {
        TRACE(CString("unable to add tooltip: ") + toolTip);
        return FALSE;
    }

    return TRUE;
}

// Assign tool tips to controls in the client area.
void CView::AssignToolTips()
{
    HWND client = *this;
    if (!m_toolTip.Create(client))
    {
        TRACE("unable to create tool tips\n");
        return;
    }

    m_toolTip.AddTool(client, L"Client area");
    AddToolTip(client, IDC_RICHEDITBOX);
    m_toolTip.Activate(TRUE);
}

// Show the view window as a modeless dialog.
HWND CView::Create(HWND parent = nullptr)
{
    m_parent = parent;
    return DoModeless(parent);
}

// This method processes all messages sent to the form dialog.
INT_PTR CView::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (WM_CTLCOLORMSGBOX <= msg && msg <= WM_CTLCOLORSTATIC)
        return OnCtlColor(msg, wparam, lparam);

    switch (msg)
    {
    case WM_SIZE:
    {
        CRect rc = GetClientRect();
        int gap = DpiScaleInt(10);
        rc.DeflateRect(gap, gap);
        GetRichView().SetWindowPos(nullptr, rc, SWP_SHOWWINDOW);
        Invalidate();
        break;  // Also do default processing
    }
    }

    // Pass unhandled messages on for default processing.
    return DialogProcDefault(msg, wparam, lparam);
}

// Announce that no document is currently open.
void CView::NoDocOpen()
{
    m_richView.SetWindowText(L"\n\n\t\tNo document is now open.\n\n\t"
        L"Load or create one using a File menu command,\n\t"
        L"or drag and drop a text file name in this area.");
}

// Here we only set the client background color, as the colors of the
// statusbar and the rich edit control are set elsewhere.See View::
// SetRichEditColors() for the latter process.
INT_PTR CView::OnCtlColor(UINT msg, WPARAM wparam, LPARAM)
{
    UINT nCtlColor = msg -  WM_CTLCOLORMSGBOX;

    // Retrieve the display context.
    CDC dcCtl(reinterpret_cast<HDC>(wparam));
    switch (nCtlColor)
    {
        case CTLCOLOR_DLG:
            return (UINT_PTR)m_bgBrush.GetHandle(); // preset by the frame

        case CTLCOLOR_MSGBOX:
        case CTLCOLOR_EDIT:
        case CTLCOLOR_LISTBOX:
        case CTLCOLOR_BTN:        // (none of these apply here)
        case CTLCOLOR_SCROLLBAR:
        case CTLCOLOR_STATIC:
            break;
    }

    return (INT_PTR)0;
}

// This method is invoked immediately before the dialog box is displayed.
// This override performs special processing to initialize the view and
// other members of the application that require all windows to have been
// created.
BOOL CView::OnInitDialog()
{
    // Load the program icons.
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Add tool tips to controls in client area.
    AssignToolTips();

    // Subclass the controls on the dialog.
    AttachItem(IDC_RICHEDITBOX, m_richView);

    // Set edit box to default font
    m_richView.SetFont(m_editFont, TRUE);

    // Put some arbitrary text in the edit control just for this demo.
    NoDocOpen();

    return TRUE;
}

// Process messages that controls send to the parent.
LRESULT CView::OnNotify(WPARAM wparam, LPARAM lparam)
{
    NMHDR* pNMH = (LPNMHDR) lparam;
    switch (pNMH->code)
    {
        case EN_DROPFILES: // A file has been dropped in the rich edit box.
            SendMessage(m_parent, WM_NOTIFY, wparam, lparam);
            return TRUE;
    }

    return CDialog::OnNotify(wparam, lparam);
}

// Invoke the page setup dialog box and set printer parameters.
void CView::OnPageSetup()
{
    MyPageSetup PSD(PSD_MARGINS);
    PSD.SetBoxTitle(L"Page Parameter Setup");
    PSD.DoModal(GetParent());

    // TODO: Add code here to set up the printer.  Note: control does not
    // return here until after OnOK() or OnCancel() have concluded.
}

// Invoke a MyPrintDialog dialog to get printing parameters and then print
// the contents of the rich view control.
void CView::OnPrintDocument(LPCWSTR path)
{
    m_richView.DoPrintRichView(path);
}

// Set the rich edit control text foreground and background colors and the
// control background color.This is needed only once(not like other
// controls set in OnCtlColor()).
void CView::SetRichEditColors(COLORREF txfg, COLORREF txbg, COLORREF bg)
{
    m_richView.SetColors(txfg, txbg, bg);
}

// Set the font for the edit box in the client window.
void CView::SetEditFont(const CFont& f)
{
    m_editFont = f;
    m_richView.SetFont(f, TRUE);
}

