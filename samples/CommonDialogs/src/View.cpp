/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CView class for the
	CommonDialogs SDI sample application using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein.

 	This particular view class contains features for selection of client
	background color, selection of edit box font, use of external
	serialization files, and context help for all controls in the client
	area.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	Special Conventions:

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CView class

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

/*============================================================================*/
	CView::
CView(UINT nResID)                             				/*

	Construct default window main view object.
*-----------------------------------------------------------------------------*/
	: CDialog(nResID)
{
}

/*============================================================================*/
	BOOL CView::
AddToolTip(HWND hParent, UINT nID)					/*

	Add the string with the resource nID to the control whose resource
	identrifer is also nID. Return TRUE on success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	return AddToolTip(hParent, nID, LoadString(nID));
}

/*============================================================================*/
	BOOL CView::
AddToolTip(HWND hParent, UINT nID, const CString & sToolTip)		/*

	Add the sToolTip string to the control whose resource identrifer is
	nID. Return TRUE on success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	HWND hCtl = ::GetDlgItem(hParent, nID);
	if (hCtl == 0)
	{
		TRACE(_T("cannot connect tooltip: ") + sToolTip);
		return FALSE;
	}
	if (!m_ToolTip.AddTool(hCtl, sToolTip))
	{
		TRACE(_T("unable to add tooltip: ") + sToolTip);
		return FALSE;
	}
	return TRUE;
}

/*============================================================================*/
	void CView::
AssignToolTips() 							/*

	Assign tool tips to all controls in the client area.
*-----------------------------------------------------------------------------*/
{
	HWND hParent = HWND((CWnd &)(*this));
	if (!m_ToolTip.Create(hParent))
	{
		TRACE(_T("unable to create tool tips\n"));
		return;
	}
	m_ToolTip.AddTool(hParent, _T("Client area"));
	  // for each control in the client area
	AddToolTip(hParent, IDOK);
	AddToolTip(hParent, IDC_RICHEDITBOX);
	  // ok, now activate these
	m_ToolTip.Activate(TRUE);
}

/*============================================================================*/
	void CView::
AttachControl(UINT nIDC, CWnd& rCtl)               			/*

	Attach (subclass) the control having the numeric identifier nID in the
	current  view object to the given CWnd rCtrl view object.  Enable the
	transfer of data between this control and the rCtl. If the rCtrl has
	already beensubclassed, do nothing.
*-----------------------------------------------------------------------------*/
{
	if ((HWND)rCtl)
		return;    // already subclassed

	HWND hWndCtrl = ::GetDlgItem((HWND)*this, nIDC);
	rCtl.Attach(hWndCtrl);
}

/*============================================================================*/
	HWND CView::
Create(HWND hParent = 0)                                               /*

	Show the view window as a modeless dialog.
**----------------------------------------------------------------------------*/
{
	m_hParent = hParent;
	return DoModeless(hParent);
}

/*============================================================================*/
	INT_PTR CView::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                     /*

	This method processes all messages sent to the form dialog. the uMsg
	parameter specifies the message and both wParam and lParam specify
	additional message-specific information. These are not used here but
	are passed to the window default procedure for further action.
**----------------------------------------------------------------------------*/
{
//	m_Resizer.HandleMessage(uMsg, wParam, lParam);

	switch (uMsg)
	{
	    case WM_SIZE:
		Invalidate();
		break;	// Also do default processing

	    case WM_LBUTTONDOWN:  // user clicks in the client area
		  // let the main frame WndProc() handle this as a click in
		  // the client window
	    	return ::SendMessage(m_hParent, IDM_HELP_ACTIVE,
		    (WPARAM)IDW_MAIN, 0);

	    case WM_CTLCOLORBTN:
	    case WM_CTLCOLOREDIT:
	    case WM_CTLCOLORDLG:
	    case WM_CTLCOLORLISTBOX:
	    case WM_CTLCOLORSCROLLBAR:
	    case WM_CTLCOLORSTATIC:
		  // let the mainframe WndProc() handle this, as well as other
		  // colors
	    	return ::SendMessage(m_hParent, uMsg, wParam, lParam);

	    case WM_DRAWITEM:
	    {
	    	LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		UINT nID = (UINT)wParam;
	    	if (nID == IDOK)
	    		m_OK.DrawItem(lpDrawItemStruct);
    		return TRUE;
	    }
	}

	  // pass unhandled messages on for default processing
	return DialogProcDefault(uMsg, wParam, lParam);
}

/*============================================================================*/
	void CView::
NoDocOpen()								/*

	Announce that no document is currently open
*-----------------------------------------------------------------------------*/
{
	m_RichEdit.SetWindowText(_T("\n\n\t\tNo document is now open.\n\n\t")
	    _T("Load or create one using a File menu command,\n\t")
	    _T("or drag and drop a text file name in this area."));
}

/*============================================================================*/
	BOOL CView::
OnCommand(WPARAM wParam, LPARAM lParam)                                 /*

	Respond to the selection of any control on the form, except one having
	the IDOK identifier. The low-order word of wParam identifies the
	command ID of the menu item, control, or accelerator. The high-order
	word of wParam specifies the notification message if the message is
	from a control. If the message is from an accelerator, the high-order
	word is 1. If the message is from a menu, the high-order word is 0. The
	lParam identifies the control that sent the message if the message is
	from a control. Otherwise, lParam is 0.
**----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	  // check all OnCommand() messages: if help mode is active, let it
	  // handle the wParam message and then return, returning TRUE;
	  // otherwise, pass control on to the body of the procedure
	if(::SendMessage(m_hParent, IDM_HELP_ACTIVE, wParam, 0))
		return TRUE;

//	UINT nID = LOWORD(wParam);
//	switch (nID)
//	{
//              TODO: add cases to respond to application-specific needs.
//
//	    default:
//	    	break;
//	}

	return FALSE;
}

/*============================================================================*/
	BOOL CView::
OnDropFiles(HDROP hDroinfo)                                            /*

	Open the text document dragged and dropped in the rich edit window.
*-----------------------------------------------------------------------------*/
{
	TCHAR szFileName[_MAX_PATH];
	::DragQueryFile((HDROP)hDroinfo, 0, (LPTSTR)szFileName, _MAX_PATH);
	GetDoc().OpenDoc(szFileName);
	return TRUE;
}

/*============================================================================*/
	BOOL CView::
OnInitDialog()                                                          /*

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
	AttachControl(IDOK, m_OK);
	AttachControl(IDC_RICHEDITBOX, m_RichEdit);
	  // set edit box to default font
	m_RichEdit.SetFont(m_EditFont, TRUE);
	  // put some arbitrary text in the edit control just for this demo
	NoDocOpen();
	return TRUE;
}

/*============================================================================*/
	LRESULT CView::
OnNotify(WPARAM wParam, LPARAM lParam)                                  /*

	Process messages that controls send to the view.
**-----------------------------------------------------------------------------*/
{
	NMHDR* pNMH;
	pNMH = (LPNMHDR) lParam;
	switch (pNMH->code)
	{
	    case EN_MSGFILTER: // keyboard or mouse event: update control states
		::SendMessage(m_hParent, IDM_UPDATECONTROLUISTATE, 0, 0);
		break;

	    case EN_DROPFILES: // a file has been dropped in the rich edit box
		ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lParam);
		HDROP hDroinfo = (HDROP) ENDrop->hDrop;
		OnDropFiles(hDroinfo);
		return TRUE;
	}

	return CDialog::OnNotify(wParam, lParam);
}

/*============================================================================*/
	void CView::
OnOK()                                                                  /*

	Override CDialog member OnOK, which is called when the IDOK message
	is activated (here when the OK button is pressed).  The base class
	default behavior is to end the dialog, which here would merely
	disable the OK button, but would not destroy the form window.
**----------------------------------------------------------------------------*/
{
	  // If help mode is active, let the main frame WndProc() handle the
	  // IDOK message; otherwise, proceed on to process OnOK event.
	if (::SendMessage(m_hParent, IDM_HELP_ACTIVE, IDOK, 0))
		return;

	::MessageBox(NULL, _T("OK Button Pressed."), _T("Information"),
	    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
	TRACE("OK Button Pressed.\n");
}

/*============================================================================*/
	void CView::
OnPrintDocument()							/*

	Invoke a MyPrinter dialog to get printing parameters and then print
	the contents of the rich view control.
*-----------------------------------------------------------------------------*/
{
	m_RichEdit.DoPrintRichView(GetDoc().GetFilePath());
}

/*============================================================================*/
	void CView::
OnPrintPreview()							/*

	Invoke the print preview dialog
*-----------------------------------------------------------------------------*/
{
	// TODO: not supported
}

/*============================================================================*/
	void CView::
PreCreate(CREATESTRUCT &cs)                                             /*

	Set defaults used by the create function for the view window for
	precise control over the window created.
*-----------------------------------------------------------------------------*/
{
	  // a child window of the main frame, with h- and v-scrolling
	cs.style = WS_CHILD;
	  // Set the extended style: a 3-D look with border and sunken edge
	cs.dwExStyle =
	    WS_EX_CONTROLPARENT       // TAB key navigation
	    ;
}

/*============================================================================*/
	void CView::
PreRegisterClass(WNDCLASS &wc)                                          /*

	Set Window class parameters for precise control over the characteristics
	of the window class created. See the WNDCLASS documentation for things
	that can be set.  Here, we mainly justset some defaults.
*-----------------------------------------------------------------------------*/
{
	  // Set the Window Class name
	wc.lpszClassName = _T("CView");
	  // Set the class style (not to be confused with the window styles
	  // set in PreCreate)
	wc.style = CS_DBLCLKS;	// Generate left button double click messages
}

/*============================================================================*/
	void CView::
Register(CDoc* pDoc)							/*

	Register the view's document.
*-----------------------------------------------------------------------------*/
{
	m_pDoc = pDoc;
}

/*============================================================================*/
	void CView::
SetRichEditColors(COLORREF txfg, COLORREF txbg, COLORREF bg)            /*


	Set the rich edit control text foreground and background colors and the
	control background color.  This is needed only once (not like other
	controls set in OnCtlColor()).
*-----------------------------------------------------------------------------*/
{
	m_RichEdit.SetColors(txfg, txbg, bg);
}

/*============================================================================*/
        void CView::
Serialize(CArchive &ar)                                               	/*

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
SetEditFont(const CFont& f)						/*

	Set the font for the edit box in the client window.
*-----------------------------------------------------------------------------*/
{
	m_EditFont = f;
	m_RichEdit.SetFont(f, TRUE);
}


