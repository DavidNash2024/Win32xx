/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: The DDXDemo CView class implementation. This class
	controls the appearance and view-related actions of the DDX/DDV test
	program. This class is a modified version of that found in the
	FormDocView smple distributed with the Win32++ Windows interface classes,
	Copyright (c) 2005-2015 David Nash, used under permissions granted
	therein. The modified sample program was based on code provided by Lynn
	Allan. This program extends the given sample by application of Dialog
	Data Exchange and Validation (DDX/DDV).

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
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

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CView class

*******************************************************************************/

#include "stdafx.h"
#include "Doc.h"
#include "View.h"
#include "App.h"
#include "resource.h"

#ifndef CFM_BACKCOLOR
#define CFM_BACKCOLOR		0x04000000
#endif // CFM_BACKCOLOR

/*=============================================================================*

	Define the color palette                                        */

#define COLOR_BLACK		RGB(  0,   0,   0)
#define COLOR_GRAY		RGB(100, 100, 100)
#define COLOR_LT_GRAY		RGB(200, 200, 200)
#define COLOR_WHITE		RGB(255, 255, 255)

#define COLOR_BLUE		RGB(  0,   0, 255)
#define COLOR_LT_BLUE		RGB(200, 200, 255)
#define COLOR_DK_BLUE		RGB(  0,   0, 128)
#define COLOR_LILAC             RGB(230, 230, 255)

#define COLOR_BROWN		RGB(255, 128,  64)
#define COLOR_DK_BROWN		RGB(128,  64,   0)

#define COLOR_YELLOW		RGB(255, 255,   0)
#define COLOR_LT_YELLOW         RGB(255, 255, 200)
#define COLOR_DK_YELLOW		RGB(128, 128,   0)

#define COLOR_CYAN		RGB(  0, 255, 255)
#define COLOR_LT_CYAN		RGB(200, 255, 255)
#define COLOR_DK_CYAN		RGB(  0, 128, 128)

#define COLOR_GREEN		RGB(  0, 255,   0)
#define COLOR_LT_GREEN		RGB(200, 255, 200)
#define COLOR_DK_GREEN		RGB(  0, 128,   0)

#define COLOR_MAGENTA		RGB(255,   0, 255)
#define COLOR_LT_MAGENTA	RGB(255, 200, 255)
#define COLOR_DK_MAGENTA	RGB(128,   0, 128)

#define COLOR_RED		RGB(255,   0,   0)
#define COLOR_LT_RED		RGB(255, 200, 200)
#define COLOR_DK_RED		RGB(128,   0,   0)


/*============================================================================*/
	CView::
CView(UINT nResID)                                                  	/*

	Construct the dialog view based on the resource.rc defined by the
	resource identifier nResID.  Set the initial text foreground and
	background display color and the form background brush for each
	control type, and set the view background color. The frame colors
	are set separately, in CMainFrame::SetThemeColors().
*-----------------------------------------------------------------------------*/
	: CDialog(nResID)
{
	// Initial values for the dialog controls			
	// These inital values need to pass DDX/DDV validation
	m_fFloat     = 0.0;	
	m_dDouble    = 0.0;	
	m_LPTSTR[0]  = _T('\0'); 
	m_iByte	     = 0;
	m_iCheckA    = 0;	
	m_iCheckB    = 0;	
	m_iCheckC    = 0;	
	m_iComboBox  = 0;	
	m_iInt       = 0;	
	m_iListBox   = 0;	
	m_iLong      = 0L;	
	m_iProgress  = 0;	
	m_iRadioA    = 0;	
	m_iScrollBar = 0;	
	m_iShort     = 0;	
	m_iSlider    = 0;	
	m_iUINT      = 10;	
	m_nIDFocus   = 0; 	
	m_ULong      = 10L;	
	CTime::GetCurrentTime().GetAsSystemTime(m_stDateTime);		
	m_stMoCalendar = m_stDateTime;					

	  // buttons
	m_clrCtlBtnFg    = COLOR_BLACK;
	m_clrCtlBtnBg    = COLOR_CYAN;
	m_brCtlBtnBg.CreateSolidBrush(COLOR_LT_CYAN);
	  // edit boxes
	m_clrCtlEditFg   = COLOR_WHITE;
	m_clrCtlEditBg   = COLOR_RED;
	m_brCtlEditBg.CreateSolidBrush(COLOR_LT_RED);
	  // the dialog box
	m_clrCtlDlgFg    = COLOR_WHITE;
	m_clrCtlDlgBg    = COLOR_BLUE;
	m_brCtlDlgBg.CreateSolidBrush(COLOR_LT_BLUE);
	  // list boxes
	m_clrCtlLBFg     = COLOR_BLACK;
	m_clrCtlLBBg     = COLOR_GREEN;
	m_brCtlLBBg.CreateSolidBrush(COLOR_LT_GREEN);
	  // scroll bars (when used)
	m_clrCtlScrlFg   = COLOR_WHITE;
	m_clrCtlScrlBg   = COLOR_MAGENTA;
	m_brCtlScrlBg.CreateSolidBrush(COLOR_LT_MAGENTA);
	  // static boxes
	m_clrCtlStatFg   = COLOR_BLACK;
	m_clrCtlStatBg   = COLOR_YELLOW;
	m_brCtlStatBg.CreateSolidBrush(COLOR_LT_YELLOW);
	  // richedit controls (these are set differently than the others)
	m_clrCtlRichEditFg = COLOR_WHITE;
	m_clrCtlRichEditBg = COLOR_RED;
	m_clrCtlRichEditBk = COLOR_LT_RED;
}

/*============================================================================*/
	CView::
~CView()								/*

*-----------------------------------------------------------------------------*/
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
	if (hCtl == NULL)
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
AdjustParameters()                    					/*

	Override this member to set the m_sStatus member set into the status
	control to reflect the values of variable controls on the dialog.
*-----------------------------------------------------------------------------*/
{
	TCHAR radio[] = {_T('A'), _T('B'), _T('C')};
	LPCTSTR	TorF[]   = {_T("false"), _T("true")};

	  // display the DateTime control content
	CTime t(m_stDateTime);
	CString dt = t.Format(_T("%d-%b-%Y %H:%M:%S"));
	  // display the MonthCalendar control content
	t = m_stMoCalendar;
 	CString mc = t.Format(_T("%d-%b-%Y"));

	  // set m_sStatus to display values of all controls
	LPCTSTR fmt = _T("STATUS Button Pressed.\nStatus is:\n\n")
	    _T("ID of focus is %d\n")
	    _T("BYTE        is %hu\n")
	    _T("short       is %hd\n")
	    _T("int         is %d\n")
	    _T("UINT        is %u\n")
	    _T("long        is %ld\n")
	    _T("ULONG       is %lu\n")
	    _T("float       is %g\n")
	    _T("double      is %g\n")
	    _T("LPTSTR      is %s\n")
	    _T("Text Box    is '%s'\n")
	    _T("RichEdit    is '%s'\n")
	    _T("ListBox     is '%s'\n")
	    _T("ComboBox    is '%s'\n")
	    _T("Radio_%c     is selected\n")
	    _T("Check Box A is %s\n")
	    _T("Check Box B is %s\n")
	    _T("Check Box C is %s\n")
	    _T("Slider Pos  is %d\n")
	    _T("ScrollBar   is %d\n")
	    _T("Progress    is %d\n")
	    _T("DateTime    is %s\n")
	    _T("Month Clndr is %s\n")
	    ;
	m_sStatus.Format(fmt,
	    m_nIDFocus,
	    m_iByte,
	    m_iShort,
	    m_iInt,
	    m_iUINT,
	    m_iLong,
	    m_ULong,
	    m_fFloat,
	    m_dDouble,
	    m_LPTSTR,
	    m_sString.c_str(),
	    m_sRichEdit.c_str(),
	    m_sListBox.c_str(),
	    m_sComboBox.c_str(),
	    radio[m_iRadioA],
	    TorF[m_iCheckA],
	    TorF[m_iCheckB],
	    TorF[m_iCheckC],
	    m_iSlider,
	    m_iScrollBar,
	    m_iProgress,
	    dt.c_str(),
	    mc.c_str()
	    );
}

/*============================================================================*/
	void CView::
AssignToolTips() 								/*

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
	AddToolTip(hParent, IDC_RADIO_A);
	AddToolTip(hParent, IDC_RADIO_B);
	AddToolTip(hParent, IDC_RADIO_C);
	AddToolTip(hParent, IDC_PUSH_ME_BUTTON);
	AddToolTip(hParent, IDC_CHECK_A);
	AddToolTip(hParent, IDC_CHECK_B);
	AddToolTip(hParent, IDC_CHECK_C);
	AddToolTip(hParent, IDOK);
	AddToolTip(hParent, IDC_LISTBOX);
	AddToolTip(hParent, IDC_EDIT_CSTRING);
	AddToolTip(hParent, IDC_EDIT_RICHEDIT);
	AddToolTip(hParent, IDC_STATUS_GROUP);
	AddToolTip(hParent, IDC_EDIT_STATUS);
	AddToolTip(hParent, IDC_ROSE_BITMAP);
	AddToolTip(hParent, IDC_EDIT_BYTE);
	AddToolTip(hParent, IDC_EDIT_SHORT);
	AddToolTip(hParent, IDC_EDIT_INT);
	AddToolTip(hParent, IDC_EDIT_UINT);
	AddToolTip(hParent, IDC_EDIT_LONG);
	AddToolTip(hParent, IDC_EDIT_DWORD);
	AddToolTip(hParent, IDC_EDIT_FLOAT);
	AddToolTip(hParent, IDC_EDIT_DOUBLE);
	AddToolTip(hParent, IDC_EDIT_LPTSTR);
	AddToolTip(hParent, IDC_CALENDAR);
	AddToolTip(hParent, IDC_DATE_TIME);
	AddToolTip(hParent, IDC_SCROLLBAR);
	AddToolTip(hParent, IDC_PROGRESSBAR);
	AddToolTip(hParent, IDC_SLIDER);
	AddToolTip(hParent, IDC_COMBOBOX);
	m_ToolTip.Activate(TRUE);
}

/*============================================================================*/
	HWND CView::
Create(HWND hParent = 0)						/*

*-----------------------------------------------------------------------------*/
{
	return DoModeless(hParent);
}

/*============================================================================*/
	INT_PTR CView::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)			/*

	This method is an application-defined callback function used with views
	derived from the CDialogBox class. It processes messages sent to modal
	and modeless dialog boxes. The uMsg parameter specifies the message,
	while both wParam and lParam specify message-specific information.
*-----------------------------------------------------------------------------*/
{
	  // Add case statements for each messages to be handled here
	switch (uMsg)
	{
	    case WM_CTLCOLORBTN:
	    case WM_CTLCOLOREDIT:
	    case WM_CTLCOLORDLG:  // sent to dialog boxes directly, not here!
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
	    	{
	    		m_StatusButton.DrawItem(lpDrawItemStruct);
	    	}
	    	if (nID == IDC_PUSH_ME_BUTTON)
	    	{
	    		m_PushButton.DrawItem(lpDrawItemStruct);
	    	}
    		return TRUE;
	    }
	}

	  // Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

/*============================================================================*/
	void  CView::
DoDataExchange(CDataExchange& DX)					/*

	Override the base class method to display and read control data for
	the CView dialog.
*-----------------------------------------------------------------------------*/
{
	CWnd::DoDataExchange(DX);  // base class does nothing
	  // attach all controls to numeric IDs first, just in case some
	  // DDV_xxxx checking on a DDX_yyyy throws an exception and leaves a
	  // later-declared DDX_Control() statement un-subclassed when
	  // initializing in READFROMCONTROL mode.
	DX.DDX_Control(IDC_COMBOBOX,       m_ComboBox);
	DX.DDX_Control(IDC_LISTBOX,        m_ListBox);
	DX.DDX_Control(IDC_EDIT_RICHEDIT,  m_RichEdit);
	DX.DDX_Control(IDOK,               m_StatusButton);
	DX.DDX_Control(IDC_PUSH_ME_BUTTON, m_PushButton);
	DX.DDX_Control(IDC_SLIDER,         m_Slider);
	DX.DDX_Control(IDC_PROGRESSBAR,    m_Progress);
	DX.DDX_Control(IDC_SCROLLBAR,      m_ScrollBar);
	DX.DDX_Control(IDC_DATE_TIME,      m_DateTime);
	DX.DDX_Control(IDC_CALENDAR,       m_MoCalendar);

	  // Now connect controls to data variables that may receive or deposit
	  // values or states

	  // Note: data in a combo box may be accessed two ways: by string or by
	  // index. Only one of these two methods should be used.  Comment the
	  // other out below.
	DX.DDX_CBString(IDC_COMBOBOX,     m_sComboBox); // use string method
//	DDX_CBIndex(IDC_COMBOBOX,	  m_iComboBox); // use index method
//	m_ComboBox.GetLBText(m_iComboBox, m_sComboBox.GetBuffer(256));
//	m_sComboBox.ReleaseBuffer();

	  // Note: data in a list box may be accessed two ways: by string or by
	  // index. Only one of these two methods should be used.  Comment the
	  // other out below.
	DX.DDX_LBString(IDC_LISTBOX,      m_sListBox);  // use string method
//	DDX_LBIndex(IDC_LISTBOX,	  m_iListBox);  // use index method
//	m_ListBox.GetText(m_iListBox, 	  m_sListBox.GetBuffer(256));
//	m_sListBox.ReleaseBuffer();

	  // connect numeric and string boxes
	  // connect byte box and check limits
	DX.DDX_Text(IDC_EDIT_BYTE,       m_iByte);
	DX.DDV_MinMaxByte(               m_iByte, 10, 200);
	  // connect short int box and check limits
	DX.DDX_Text(IDC_EDIT_SHORT,      m_iShort);
	DX.DDV_MinMaxShort(              m_iShort, -1000, 1000);
	  // connect int box and check limits
	DX.DDX_Text(IDC_EDIT_INT,        m_iInt);
	DX.DDV_MinMaxInt(                m_iInt, -10000, 10000);
	  // connect UINT box and check limits
	DX.DDX_Text(IDC_EDIT_UINT,       m_iUINT);
	DX.DDV_MinMaxUInt(               m_iUINT, 10, 10000);
	  // connect long box and check limits
	DX.DDX_Text(IDC_EDIT_LONG,       m_iLong);
	DX.DDV_MinMaxLong(               m_iLong, -100000, 100000);
	  // connect ULONG box and check limits
	DX.DDX_Text(IDC_EDIT_DWORD,      m_ULong);
	DX.DDV_MinMaxULong(              m_ULong, 10, 100000);
	  // connect float box and check limits
	DX.DDX_Text(IDC_EDIT_FLOAT,      m_fFloat);
	DX.DDV_MinMaxFloat(              m_fFloat, -10.0, 100000.0);
	  // connect double box and check limits
	DX.DDX_Text(IDC_EDIT_DOUBLE,     m_dDouble);
	DX.DDV_MinMaxDouble(             m_dDouble, -10.0, 100000.);
	  // connect the LPTSTR box and check length
	DX.DDX_Text(IDC_EDIT_LPTSTR,     m_LPTSTR, 255);
	DX.DDV_MaxChars(                 m_LPTSTR, 25);
	  // connect the regular edit box and check length
	DX.DDX_Text(IDC_EDIT_CSTRING,    m_sString);
	DX.DDV_MaxChars(                 m_sString, 25);
	  // connect the rich edit box for string operations
	DX.DDX_Text(IDC_EDIT_RICHEDIT,   m_sRichEdit);
	DX.DDV_MaxChars( 	             m_sRichEdit, 25); // limit length
	  // connect the slider control
	DX.DDX_Slider(IDC_SLIDER,        m_iSlider);
	DX.DDV_MinMaxSlider(             m_iSlider, 0, 1000);
	  // connect the progress bar
	DX.DDX_Progress(IDC_PROGRESSBAR, m_iProgress);
	  // connect scroll bar
	DX.DDX_Scroll(  IDC_SCROLLBAR,   m_iScrollBar);
	  // connect the radio boxes
	DX.DDX_Radio( IDC_RADIO_A,       m_iRadioA);
	  // connect check boxes
	DX.DDX_Check(IDC_CHECK_A,        m_iCheckA);
	DX.DDX_Check(IDC_CHECK_B,        m_iCheckB);
	DX.DDX_Check(IDC_CHECK_C,        m_iCheckC);
	  // engage the DateTime control and check +/-30 days
	CTime tnow = CTime::GetCurrentTime();
	CTimeSpan tmo = 30 * 86400;
	CTime   tMin = tnow - tmo;
	CTime   tMax = tnow + tmo;
	SYSTEMTIME stMin, stMax;
	tMin.GetAsSystemTime(stMin);
	tMax.GetAsSystemTime(stMax);
	DX.DDX_DateTime(IDC_DATE_TIME,   m_stDateTime);
	DX.DDV_MinMaxDateTime(           m_stDateTime,  stMin, stMax);
	  // engage the Month Calendar control and check +/-30 days
	DX.DDX_MonthCal(IDC_CALENDAR,    m_stMoCalendar);
	DX.DDV_MinMaxMonth(              m_stMoCalendar,  stMin, stMax);
	  // connect the status box
	DX.DDX_Text(IDC_EDIT_STATUS,     m_sStatus);
}

/*============================================================================*/
	void CView::
GetDocumentValues() 							/*

	Load those things from the document that it saves.
*-----------------------------------------------------------------------------*/
{
 	m_iByte     = TheDoc().GetByte();
	m_iShort    = TheDoc().GetShort();
	m_iInt      = TheDoc().GetInt();
	m_iUINT     = TheDoc().GetUINT();
	m_iLong     = TheDoc().GetLong();
	m_ULong     = TheDoc().GetULong();
	m_fFloat    = TheDoc().GetFloat();
	m_dDouble   = TheDoc().GetDouble();
	m_iRadioA   = TheDoc().GetRadio();
	m_iCheckA   = TheDoc().GetCheckA();
	m_iCheckB   = TheDoc().GetCheckB();
	m_iCheckC   = TheDoc().GetCheckC();
	m_sString   = TheDoc().GetEditBox();
	m_sRichEdit = TheDoc().GetRichEditBox();
	m_sListBox  = TheDoc().GetListBoxS();
	m_iListBox  = TheDoc().GetListBoxX();
	m_sComboBox = TheDoc().GetComboBoxS();
	m_iComboBox = TheDoc().GetComboBoxX();
	m_iSlider   = TheDoc().GetSlider();
	m_stDateTime   = TheDoc().GetDateTime();
	m_stMoCalendar = TheDoc().GetMoCalendar();
	_tcscpy(m_LPTSTR, TheDoc().GetLPTSTR());
}

/*============================================================================*/
	void CView::
OnBitmap()								/*

	Activate the DDX/DDV mechanism to read current values from the dialog
	form and to set the status box to indicate this button was pressed.
*-----------------------------------------------------------------------------*/
{
	  // save current contents of controls

	UpdateDialog(READFROMCONTROL);
	  // reset status to just this message:
	m_sStatus = _T("The moondance rose.");
	  // post the message
	UpdateDialog(SENDTOCONTROL);
	TRACE("Bitmap Pressed\n");
}

/*============================================================================*/
	void CView::
OnButton()								/*

	Activate the DDX/DDV mechanism to read current values from the dialog
	form and to set the status box to indicate this button was pressed.
*-----------------------------------------------------------------------------*/
{
	  // read current contents of controls to memory
	UpdateDialog(READFROMCONTROL);
	  // reset the status message to just this:
	m_sStatus = _T("PUSH ME button Pressed");
	  // send this status message (and also the other values) back into
	  // the control
	UpdateDialog(SENDTOCONTROL);
	TRACE("PUSH ME button Pressed\n");
}

/*============================================================================*/
	BOOL CView::
OnCommand(WPARAM wParam, LPARAM lParam)					/*

	The framework calls this member function when the user selects an item
	from a menu, when a child control sends a notification message, or when
	an accelerator keystroke is translated. On recognizing a particular
	message, the appropriate member function is to be activated. This method
	allows your application to handle Windows messages. The parameters
	passed above reflect the parameters received by the framework when the
	message was received. If you call the base-class implementation of this
	function, that implementation will use the parameters originally passed
	with the message and not the parameters you supply to the function.
	The method should return nonzero if it processes a particular message;
	otherwise it should return 0.  The low-order word of wParam identifies
	the command ID of the menu item, control, or accelerator. The high-order
	word of wParam specifies the notification message if the message is from
	a control. If the message is from an accelerator, the high-order word
	is 1. If the message is from a menu, the high-order word is 0.  The
	lParam identifies the control that sends the message if the message is
	from a control. Otherwise, lParam is 0.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);
	WORD nID  = LOWORD(wParam);
	WORD hCtl = HIWORD(wParam);
	switch (nID)
	{
	    case IDC_PUSH_ME_BUTTON:
		OnButton();
		m_nIDFocus = nID;
		return TRUE;

	    case IDC_ROSE_BITMAP:
		OnBitmap();
		m_nIDFocus = nID;
		return TRUE;
	}

	  // deal with setting the focus for edit controls, combo boxes,
	  // list boxes, radio buttons, and check boxes
	if (hCtl == EN_SETFOCUS || hCtl == CBN_SETFOCUS || hCtl == LBN_SETFOCUS
	    || (hCtl == BN_SETFOCUS && IDC_RADIO_A <= nID && nID <= IDC_CHECK_C))
	{
		m_nIDFocus = nID;
		return FALSE;
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
	  // to avoid compiler warnings, declare control colors and
	  // assign them arbitrary (because unused) values
	COLORREF fg = COLOR_BLACK,
		 bk = COLOR_BLACK;
	CBrush   br = CBrush(COLOR_BLACK);
	  // handle each color message separtely
	switch (nCtlColor)
	{
	    case WM_CTLCOLORBTN:
		  // handle each button separately (but here, they have the
		  // same colors)
	    	if (nID == IDOK)
	    	{
			fg = m_clrCtlBtnFg;
	    		bk = m_clrCtlBtnBg;
	    		br = m_brCtlBtnBg;
		}
	    	if (nID == IDC_PUSH_ME_BUTTON)
	    	{
			fg = m_clrCtlBtnFg;
	    		bk = m_clrCtlBtnBg;
	    		br = m_brCtlBtnBg;
		}
		break;

	    case WM_CTLCOLOREDIT:
	    	fg = m_clrCtlEditFg;
	    	bk = m_clrCtlEditBg;
	    	br = m_brCtlEditBg;
		break;

	    case WM_CTLCOLORDLG:
	    	fg = m_clrCtlDlgFg;
	    	bk = m_clrCtlDlgBg;
	    	br = m_brCtlDlgBg;
		break;

	    case WM_CTLCOLORLISTBOX:
	    	fg = m_clrCtlLBFg;
	    	bk = m_clrCtlLBBg;
	    	br = m_brCtlLBBg;
		break;

	    case WM_CTLCOLORSCROLLBAR:
	    	fg = m_clrCtlScrlFg;
	    	bk = m_clrCtlScrlBg;
	    	br = m_brCtlScrlBg;
		break;

	    case WM_CTLCOLORSTATIC:
		  // change caption color of group box
		if (nID == IDC_STATUS_GROUP)
		{
			fg = m_clrCtlDlgFg;
			bk = m_clrCtlDlgBg;
			br = m_brCtlDlgBg; // doesn't do anything
		}
		else if (IDC_RADIO_A <= nID && nID <= IDC_CHECK_C)
		{
			fg = m_clrCtlBtnFg;
	    		bk = m_clrCtlBtnBg;
	    		br = m_brCtlBtnBg;
			break;
		}
		else
		{
		    	fg = m_clrCtlStatFg;
		    	bk = m_clrCtlStatBg;
		    	br = m_brCtlStatBg;
			break;
		}
	}

	dcCtl.SetTextColor(fg);
	dcCtl.SetBkColor(bk);
	  // returned brush handle must be to permanent brush
        return (INT_PTR)(HBRUSH)br;
}

/*============================================================================*/
	BOOL CView::
OnInitDialog()								/*

	This member function is activated in response to the WM_INITDIALOG
	message, which is sent to the dialog box during the CreateIndirect,
	Create, or DoModal calls that occur immediately before the dialog box
	is displayed. Insert instructions that are needed to perform special
	processing when the dialog box is initialized. The base class does
	nothing, and need not be invoked.

	Windows calls this function via a standard global dialog-box procedure,
	rather than via your message map, so no OnCommand() message entry for
	this member function is issued.

	The return value should indicate whether the application has set the
	input focus to one of the controls in the dialog box. If nonzero,
	Windows sets the input focus to the first control in the dialog box.
	The application should return 0 only if it has explicitly set the input
	focus to one of the controls in the dialog box.
*-----------------------------------------------------------------------------*/
{
	  // Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);
	  // add tool tips to controls in client area
	AssignToolTips();
	  // Connect controls to IDs and read default data values into them.
	UpdateDialog(SENDTOCONTROL);

	  // Set the rich edit control text foreground and background colors
	  // and the control background color.  This is needed only once (not
	  // like other controls set in OnCtlColor()).
	CHARFORMAT2 chf;
        chf.cbSize = sizeof(chf);
        chf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
        chf.dwEffects = 0;
	chf.crTextColor = m_clrCtlRichEditFg;
        chf.crBackColor = m_clrCtlRichEditBg;
	m_RichEdit.SetDefaultCharFormat(chf);
	m_RichEdit.SetBackgroundColor(FALSE, m_clrCtlRichEditBk);

	  // initialize some of the subclassed controls for this test:
	  // list box, combo box, slider, progress bar, and scroll bar:

	  // put some text in the list box (normally, entries might be
	  // retrieved from the document, saved in the registry, but here,
	  // for simplplicity, we just enter some sample values)
	m_ListBox.ResetContent();
	for (int i = 0 ; i < 10 ; i++)
	{
		CString s;
		s.Format(_T("List Box%d"), i);
		m_ListBox.AddString(s);
	}
	  // ditto for the combo box
	m_ComboBox.ResetContent();
	for (int j = 0 ; j < 8 ; j++)
	{
		CString s;
		s.Format(_T("Combo Box%d"), j);
		m_ComboBox.AddString(s);
	}
	  // set the slider, progress, and scroll bar ranges
	int	lo   = 0,
		page = 20,
		hi   = 50 * page;
	m_ScrollBar.SetScrollInfo(lo, hi, 0, page);
	  // set slider range and page size
	m_Slider.SetRangeMin(lo);
	m_Slider.SetRangeMax(hi);
	m_Slider.SetPageSize(page);
	m_Progress.SetRange((short)lo, (short)hi);
	  // set Date-Time display format
	m_DateTime.SetFormat(_T("dd'-'MMM'-'yyyy' 'HH':'mm':'ss"));

	  // retrieve values from the document
	GetDocumentValues();
	  // gang together progress, scroll bar, and slider
	m_iProgress = m_iScrollBar = m_iSlider;

	return TRUE;
}

/*============================================================================*/
	void CView::
OnOK()									/*

	Called when the user clicks the OK button (the button with an ID of
	IDOK).  Show the contents of all controls and save those that are part
	of the document. Override this member function to alter the OK button
	action. The implementation here reads, validates, and updates the
	dialog-box data and saves the parameters to the application, as
	necessary.

	Note: If the OK button is implemented within in a modeless dialog and
	destruction of the dialog is desired on execution of OnOK(), override
	this method and call DestroyWindow() from within that override. Do not
	call the base-class member function CDialog::OnOK(), because it calls
	EndDialog(), which merely makes the dialog box invisible or inoperative,
	but does not destroy it.
*-----------------------------------------------------------------------------*/
{
	UpdateDialog(READFROMCONTROL);
	AdjustParameters();
	UpdateDialog(SENDTOCONTROL);
	UpdateParameters();

	TRACE("STATUS Button Pressed.\n\n");
}

/*============================================================================*/
	LRESULT CView::
OnNotify(WPARAM wParam, LPARAM lParam)                                  /*

	Called when a WM_NOTIFY message is received from a child window. This
	override handles these notifications.
*-----------------------------------------------------------------------------*/
{
	NMHDR *pNMHdr = (LPNMHDR)lParam;

	  // deal with setting the focus for date-time controls
	if (pNMHdr->code == NM_SETFOCUS)
	{
		m_nIDFocus = (int)wParam;
		return TRUE;
	}
	return FALSE;
}

/*============================================================================*/
	BOOL CView::
PreTranslateMessage(MSG* pMsg)                              		/*

	Filter mouse and keyboard messages prior to being passed to the
	message loop.
*-----------------------------------------------------------------------------*/
{
	if ((HWND)m_ToolTip != NULL)
		m_ToolTip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

/*============================================================================*/
	BOOL	CView::
UpdateDialog(BOOL bReadFromControl /* = SENDTOCONTROL */)		/*
    
	Update data items in memory (bReadFromControl is READFROMCONTROL) or in
	dialog controls (bReadFromControl is SENDTOCONTROL). The list of
	affected controls is specified in the DoDataExchange() method.
*-----------------------------------------------------------------------------*/
{
	CMyDataExchange DX;
	return UpdateData(DX, bReadFromControl);
}

/*============================================================================*/
	void CView::
UpdateParameters()							/*

	Override this member to save the pertinent members of the view that
	are also members of the document.
*-----------------------------------------------------------------------------*/
{
	TheDoc().SetByte(m_iByte);
	TheDoc().SetShort(m_iShort);
	TheDoc().SetInt(m_iInt);
	TheDoc().SetUINT(m_iUINT);
	TheDoc().SetLong(m_iLong);
	TheDoc().SetULong(m_ULong);
	TheDoc().SetFloat(m_fFloat);
	TheDoc().SetDouble(m_dDouble);
	TheDoc().SetLPTSTR(m_LPTSTR);
	TheDoc().SetEditBox(m_sString);
	TheDoc().SetRichEditBox(m_sRichEdit);
	TheDoc().SetListBoxS(m_sListBox);
	TheDoc().SetListBoxX(m_iListBox);
	TheDoc().SetComboBoxS(m_sComboBox);
	TheDoc().SetComboBoxX(m_iComboBox);
	TheDoc().SetRadio(m_iRadioA);
	TheDoc().SetCheckA(m_iCheckA);
	TheDoc().SetCheckB(m_iCheckB);
	TheDoc().SetCheckC(m_iCheckC);
	TheDoc().SetSlider(m_iSlider);
	TheDoc().SetDateTime(m_stDateTime);
	TheDoc().SetMoCalendar(m_stMoCalendar);
}

