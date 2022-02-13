/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
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

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

/*============================================================================*/
    CView::
CView(UINT id)                                                              /*

    Construct default window main view object.
*-----------------------------------------------------------------------------*/
    : CDialog(id), m_parent(0)
{
}

/*============================================================================*/
    BOOL CView::
AddToolTip(HWND parent, UINT id)                                            /*

    Add the string with the resource id to the control whose resource
    identifier is also id. Return TRUE on success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    return AddToolTip(parent, id, LoadString(id));
}

/*============================================================================*/
    BOOL CView::
AddToolTip(HWND parent, UINT id, LPCTSTR toolTip)                          /*

    Add the toolTip string to the control whose resource identifier is
    id. Return TRUE on success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    HWND hCtl = ::GetDlgItem(parent, id);
    if (hCtl == 0)
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

/*============================================================================*/
    void CView::
AssignToolTips()                                                            /*

    Assign tool tips to controls in the client area.
*-----------------------------------------------------------------------------*/
{
    HWND client = HWND((CWnd &)(*this));
    if (!m_toolTip.Create(client))
    {
        TRACE(_T("unable to create tool tips\n"));
        return;
    }
    m_toolTip.AddTool(client, _T("Client area"));
      // for the only control in the client area
    AddToolTip(client, IDC_RICHEDITBOX);
      // ok, now activate these
    m_toolTip.Activate(TRUE);
}

/*============================================================================*/
    HWND CView::
Create(HWND parent = 0)                                                    /*

    Show the view window as a modeless dialog.
**----------------------------------------------------------------------------*/
{
    m_parent = parent;
    return DoModeless(parent);
}

/*============================================================================*/
    INT_PTR CView::
DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)                         /*

    This method processes all messages sent to the form dialog. the msg
    parameter specifies the message and both wparam and lparam specify
    additional message-specific information. These are not used here but
    are passed to the window default procedure for further action.
**----------------------------------------------------------------------------*/
{
    if (WM_CTLCOLORMSGBOX <= msg && msg <= WM_CTLCOLORSTATIC)
        return OnCtlColor(msg, wparam, lparam);

    switch (msg)
    {
        case WM_SIZE:
        Invalidate();
        break;  // Also do default processing
    }
      // pass unhandled messages on for default processing
    return DialogProcDefault(msg, wparam, lparam);
}

/*============================================================================*/
    void CView::
NoDocOpen()                                                                 /*

    Announce that no document is currently open.
*-----------------------------------------------------------------------------*/
{
    m_richView.SetWindowText(_T("\n\n\t\tNo document is now open.\n\n\t")
        _T("Load or create one using a File menu command,\n\t")
        _T("or drag and drop a text file name in this area."));
}

/*============================================================================*/
    INT_PTR CView::
OnCtlColor(UINT msg, WPARAM wparam, LPARAM lparam)                            /*

    Normally here we set the foreground and background colors of controls
    given their color type identifier msg, their hDC display context
    (HDC)wparam, and their handle (HWND)lparam, and then return an HBRUSH
    cast to an INT_PTR for the color to be used to paint the control. However,
    in this case, only the client background color is set, as the colors of the
    statusbar and the rich edit control are set elsewhere. See View::
    SetRichEditColors() for the latter process.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);
    UINT nCtlColor = msg -  WM_CTLCOLORMSGBOX;
      // get the display context
    CDC dcCtl((HDC)wparam);
    switch (nCtlColor)
    {
        case CTLCOLOR_DLG:
            return (UINT_PTR)(HBRUSH)m_bgBrush; // preset by the frame

        case CTLCOLOR_MSGBOX:
        case CTLCOLOR_EDIT:
        case CTLCOLOR_LISTBOX:
        case CTLCOLOR_BTN:        // (none of these apply here)
        case CTLCOLOR_SCROLLBAR:
        case CTLCOLOR_STATIC:
            break;
    }
      // else, not handled here
    return (INT_PTR)0;
}

/*============================================================================*/
    BOOL CView::
OnInitDialog()                                                              /*

    This method is invoked immediately before the dialog box is displayed.
    This override performs special processing to initialize the view and
    other members of the application that require all windows to have been
    created. The method always returns TRUE.
*-----------------------------------------------------------------------------*/
{
      // first call the base class but disregard its return value
    CDialog::OnInitDialog();
      // load the program icons
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);
      // add tool tips to controls in client area
    AssignToolTips();
      // subclass the controls on the dialog
    AttachItem(IDC_RICHEDITBOX, m_richView);
      // set edit box to default font
    m_richView.SetFont(m_editFont, TRUE);
      // put some arbitrary text in the edit control just for this demo
    NoDocOpen();
    return TRUE;
}

/*============================================================================*/
    LRESULT CView::
OnNotify(WPARAM wparam, LPARAM lparam)                                      /*

    Process messages that controls send to the parent.
**-----------------------------------------------------------------------------*/
{
    NMHDR* pNMH = (LPNMHDR) lparam;
    switch (pNMH->code)
    {
        case EN_DROPFILES: // a file has been dropped in the rich edit box
            SendMessage(m_parent, WM_NOTIFY, wparam, lparam);
            return TRUE;
    }

    return CDialog::OnNotify(wparam, lparam);
}

/*============================================================================*/
    void CView::
OnPageSetup()                                                               /*

    Invoke the page setup dialog box and set printer parameters.
*-----------------------------------------------------------------------------*/
{
    MyPageSetup PSD(PSD_MARGINS);
    PSD.SetBoxTitle(_T("Page Parameter Setup"));
    PSD.DoModal(GetParent());

    // TODO: Add code here to set up the printer.  Note: control does not
    // return here until after OnOK() or OnCancel() have concluded.
}

/*============================================================================*/
    void CView::
OnPrintDocument(LPCTSTR path)                                               /*

    Invoke a MyPrintDialog dialog to get printing parameters and then print
    the contents of the rich view control.
*-----------------------------------------------------------------------------*/
{
    m_richView.DoPrintRichView(path);
}

/*============================================================================*/
    void CView::
SetRichEditColors(COLORREF txfg, COLORREF txbg, COLORREF bg)                /*


    Set the rich edit control text foreground and background colors and the
    control background color.  This is needed only once (not like other
    controls set in OnCtlColor()).
*-----------------------------------------------------------------------------*/
{
    m_richView.SetColors(txfg, txbg, bg);
}

/*============================================================================*/
        void CView::
Serialize(CArchive &ar)                                                     /*

        Called to serialize or deserialize the view to and from the archive ar,
        depending on the sense of IsStoring().
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    if (ar.IsStoring())
    {
    }
    else    // recovering
    {
    }
}

/*============================================================================*/
    void CView::
SetEditFont(const CFont f)                                                  /*

    Set the font for the edit box in the client window.
*-----------------------------------------------------------------------------*/
{
    m_editFont = f;
    m_richView.SetFont(f, TRUE);
}
/*----------------------------------------------------------------------------*/
