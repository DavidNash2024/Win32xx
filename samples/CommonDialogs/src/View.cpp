/* (03-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
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

	Programming Notes: The representation of a font in this class takes the
	form of a MyFontDialog class object. This format allows access to not
	only the font itself, but to its background and foreground colors and
	to its average heigh and width, as well. It allows user selection of 
	font characteristics and access to assistance via the HELP button  in
	the selection process. Serialization of the object provides an easy
	means to archive all of the font's attributes.
	
        The programming standards roughly follow those established by the
	1997-1999 Jet Propulsion Laboratory Deep Space Network Planning and
	Preparation Subsystem project for C++ programming.
********************************************************************************

	Implementation of the CView class

*******************************************************************************/

#include "stdafx.h"
#include "App.h"
#include "ColorDefs.h"
#include "Resource.h"
#include "ContextHelp.h"
#include "resource.h"

/*******************************************************************************

	Macros, constants, and local (static) default constants 	*/

  // latest file compilation date
CString CView::m_sCompiled_on = __DATE__;

/*============================================================================*/
	enum
CustomColors
{
	OKTxFg = EndDefaultColors, OKTxBg,  OKBg,  // OK custom button
	EndCustomColors
};

/*============================================================================*/
	CView::
CView(UINT nResID)                             				/*

	Construct default window main view object.
*-----------------------------------------------------------------------------*/
	: CDialog(nResID)
{
	m_cWd = m_cHt  = 0;
	CFont f;
	  // 10 pt, Courier default font
	m_FontChoice.SetChoiceFont(f.CreatePointFont(100, _T("Courier New")));
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
	if (!m_ToolTip.AddTool(hCtl, sToolTip.c_str()))
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
	AddToolTip(hParent, IDM_EDITBOX);
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
	    {
		  // let the main frame OnCommand() handle this as a
		  // notification from the main window
	    	return theFrame.SendMessage(WM_COMMAND, (WPARAM)IDW_MAIN,
		    (LPARAM)0);
	    }

	    case WM_CTLCOLORBTN:
	    case WM_CTLCOLOREDIT:
	    case WM_CTLCOLORDLG:
	    case WM_CTLCOLORLISTBOX:
	    case WM_CTLCOLORSCROLLBAR:
	    case WM_CTLCOLORSTATIC:
		  // for these messages, wParam has the control's hdc and
		  // lParam has the control's hwnd
		return OnCtlColor((HDC)wParam, (HWND)lParam, uMsg);

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
OnColorChoice() 							/*

	Show the control color choice dialog box and select a control color.
	The CtlColorChoice class
*-----------------------------------------------------------------------------*/
{
	  // set color choice help messages to go to the the main frame,
	HWND hWndOwner = theApp.GetMainWnd();
	m_CtlColorChoice.DoModal(hWndOwner);  // calls the base class DoModal()
}

/*============================================================================*/
	BOOL CView::
OnCommand(WPARAM wParam, LPARAM lParam)                                 /*

	Respond to the selection of any control on the form, except one having
	the IDOK identifier. The low-order word of wParam identifies the
	command ID of the menu item, control, or accelerator. The high-order
	word of wParam specifies the notification message if the message is
	from a control. If the message is from an accelerator, the high-order
	word is 1. If the message is from a menu, the high-order word is 0.
**----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	  // check all OnCommand() messages: if help mode is active, let it
	  // handle the wParam message
	if (theFrame.OnContextHelp(wParam))
		return TRUE; // context help active

	UINT nID = LOWORD(wParam);
	switch (nID)
	{
	      // handle help request messages sent to the view from
	      // common dialogs (if any)
	    case IDC_HELP_COMDLG:
	    	theFrame.EngageContextHelp();
	    	return theFrame.OnContextHelp((WPARAM)lParam);

	    default:
	    	break;
	}

	return FALSE;
}

/*============================================================================*/
	INT_PTR  CView::
OnCtlColor(HDC hDC, HWND hWnd, UINT nCtlColor)            		/*

	This member function is invoked when a child control is about to be
	drawn. It can be used to prepare the display context hDC for drawing
	the control using user-selected colors. Here, the SetTextColor() and
	SetBkColor() methods may be used with desired RGB values to set the
	text and background colors of almost all controls.  Here, hDC contains
	the handle to the display context for the child window, which may be
	temporary.  hWnd contains he handle to the control asking for the
	color, which also may be temporary. Finally, nCtlColor contains one
	of the messages specifying the type of control (see DialogProc() for
	values). On termination the method returns a handle to the brush that
	is to be used for painting the control background.

	Note: This method is not called for the list box of a drop-down combo
	box because the drop-down list box is actually a child of the combo box
	and not a child of the window. To change the color of the drop-down
	list box, create a CComboBox with a similar OnCtlColor() that checks
	for CTLCOLOR_LISTBOX in the nCtlColor parameter. In that handler, the
	SetBkColor() member function must be used to set the background color
	for the text.
*-----------------------------------------------------------------------------*/
{
	  // get the control numeric ID, when needed
	UINT nID = ::GetDlgCtrlID(hWnd);
	  // get the display context
	CDC dcCtl(hDC);
	  // declare default control colors and brush
	COLORREF fg = COLOR_BLACK,
		 bk = COLOR_BLACK;
	CBrush   br = CBrush(COLOR_BLACK);

	  // handle colors of each control class first
	m_CtlColorChoice.GetContolColors(nCtlColor, fg, bk, br);
	  // if there are custom colors for some controls, change them here
	switch (nCtlColor)
	{
	    case WM_CTLCOLORBTN:
		  // As illustrated here, we can handle each control separately
		  // according to its nID. For the OK button case
	    	if (nID == IDOK)
	    	{
			fg = m_CtlColorChoice.GetColor(OKTxFg);
	    		bk = m_CtlColorChoice.GetColor(OKTxBg);
	    		br = m_CtlColorChoice.GetBrush(OKBg);
		}
		// ... ditto for other buttons
		break;

	    // ditto for other controls in other classes
	}

	dcCtl.SetTextColor(fg);
	dcCtl.SetBkColor(bk);
	  // returned brush handle must be to permanent brush
        return (UINT_PTR)(HBRUSH)br;
}

/*============================================================================*/
	void CView::
OnFontChoice()     		                                 	/*

        Select the view font typeface, characteristics, and color.
*-----------------------------------------------------------------------------*/
{
	  // set font choice help messages to go to the the main frame,
	HWND hOwnerWnd = GetApp().GetMainWnd();
          // open the dialog
	m_FontChoice.SetBoxTitle(_T("Select font for edit box"));
	if(m_FontChoice.DoModal(hOwnerWnd) == IDOK)
	{
		  // bring choice elements into this view
                m_Edit.SetFont(m_FontChoice.GetChoiceFont(), TRUE);
		m_cWd = m_FontChoice.GetAvgSize().cx;
		m_cHt = m_FontChoice.GetAvgSize().cy;
		m_CtlColorChoice.SetColorTable(EdtTxFg, m_FontChoice.GetColor());
	}
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
	AttachControl(IDM_EDITBOX, m_Edit);
	  // set default or recovered font
	m_Edit.SetFont(m_FontChoice.GetChoiceFont(), TRUE);
	  // Put some arbitrary initial text in the edit control just for
	  // this demo. It gets overwritten, so it is never seen on screen.
	m_Edit.SetWindowText(_T("hello world"));
	  // By design MyFontDlg help messages to go to the main frame, show the
	  // help box, use the previous font and font style. Here set a default
	  // choice color
	m_FontChoice.SetColor(COLOR_BLACK);
	  // Set color choice dialog initial flags to show the help box and all
	  // colors
	CHOOSECOLOR cc = m_CtlColorChoice.GetParameters();
	cc.Flags = CC_SHOWHELP | CC_FULLOPEN;
	cc.Flags |= CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
	m_CtlColorChoice.SetParameters(cc);

	// add custom control colors, if not already there
	if (m_CtlColorChoice.GetTableSize() == EndDefaultColors)
	{
		m_CtlColorChoice.AddColorChoice(_T("x  OK text foreground"),
		    COLOR_BLACK);
		m_CtlColorChoice.AddColorChoice(_T("x  OK text background"),
		    COLOR_YELLOW);
		m_CtlColorChoice.AddColorChoice(_T("x  OK background"),
		    COLOR_LT_YELLOW);
	}
	return TRUE;
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
	  // if help mode is active, let it handle the IDOK message
	if (theFrame.OnContextHelp((WPARAM)IDOK))
		return;

	::MessageBox(NULL, _T("OK Button Pressed."), _T("Information"),
	    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
	TRACE("OK Button Pressed.\n");
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
	BOOL CView::
PreTranslateMessage(MSG& Msg)                                           /*

	Used by CWinApp to translate window messages before they are dispatched
	to theTranslateMessage and DispatchMessage Windows functions in the
	message loop. MSG contains the message to process. Return a nonzero
	if the message was translated and should not be dispatched; return
	0 if the message was not translated and should be dispatched.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(Msg);

//	HWND   hwnd	= Msg->hwnd;
//	UINT   message	= Msg->message;
//	WPARAM wParam	= Msg->wParam;
//	LPARAM lParam	= Msg->lParam;
//	DWORD  time	= Msg->time;
//	CPoint  pt	= Msg->pt;

	  // return 0 if the message was NOT handled here
	return 0;
}

/*============================================================================*/
        void CView::
Serialize(CArchive &ar)                                               	/*

        Called to serialize or deserialize the view to and from the archive ar,
        depending on the sense of IsStoring().  Leaves the archive open for
        for further serialization.
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
                  // save font parameters
		ar << m_FontChoice;
		  // save regular control class colors
		ar << m_CtlColorChoice;
	}
        else    // recovering
        {
                  // recover font parameters
		ar >> m_FontChoice;
		m_Edit.SetFont(m_FontChoice.GetChoiceFont(), TRUE);
		m_cWd = m_FontChoice.GetAvgSize().cx;
		m_cHt = m_FontChoice.GetAvgSize().cy;
		  // recover colors
		ar >> m_CtlColorChoice;
      	}
}


