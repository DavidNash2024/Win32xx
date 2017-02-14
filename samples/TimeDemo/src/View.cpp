/* (03-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CView class for a
	skeleton SDI application using the Win32++ Windows interface classes,
	Copyright (c) 2005-2015 David Nash, under permissions granted therein.

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
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Implementation of the CView class

*******************************************************************************/

#include "stdafx.h"
#include "View.h"
#include "Doc.h"
#include "App.h"

/*******************************************************************************

	Class static variable initialization				*/

  // latest file compilation date
const CString CView::m_sCompiled_on = __DATE__;

/*============================================================================*/
	CView::
CView()                             					/*

	Construct default window main view object.
*-----------------------------------------------------------------------------*/
{
	m_cWd = m_cHt  = 0;
	m_rgbTxColor   = RGB(0,0,0);
        m_rgbBkColor   = RGB(255,255,128);
        m_rgbTxHiColor = RGBHighlight(m_rgbTxColor);
	m_rgbBkHiColor = RGBHighlight(m_rgbBkColor);
	m_hCursor      = ::LoadCursor(0, IDC_ARROW);
	SetCursorShape(arrow);
}

/*============================================================================*/
	CView::
~CView()                                                            	/*

	Destroy window main view.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CPoint CView::
GetClientWdHt()                                            		/*

	Calculate the number of horizontal and  vertical units present in the
	view rectangle, rounded to the nearest integer value.  Return the
	results as a CPoint structure.
*-----------------------------------------------------------------------------*/
{
	  // TODO: change this to fit the application:
	  
	  // Here we count lines and  columns of the current font, rounded
	  // to nearest integer.
	RECT rc = GetClientRect();
	CPoint pt;
	pt.x = (rc.right - rc.left + m_cWd / 2) /  m_cWd;
	pt.y = (rc.bottom - rc.top + m_cHt / 2) /  m_cHt;
	return pt;
}

/*============================================================================*/
	int	CView::
OnCreate(CREATESTRUCT& cs) 						/*

*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(cs);  

	// TODO: add code here  to prepare the initial view
	SetDefaultFont();

	  // attach scroll bars to this view
	m_sb.Register(this);
	  // hide scroll bars for now
	m_sb.ShowHScrollBar(FALSE);
	m_sb.ShowVScrollBar(FALSE);

	return 0;
}

/*============================================================================*/
	void CView::
OnDraw(CDC& dc)                                                        /*

	RedDraw the client area window. This is done whenever a part or all
	of the client window needs to be redrawn.
*-----------------------------------------------------------------------------*/
{
	  // The drawing of the client area is done in a manner that reduces
	  // flicker when the window is scrolled or resized. This is done by
	  // creating a compatible DC and bitmap, painting what is to appear
	  // in the view into this bitmap, and, once complete, copying this
	  // bitmap into the view.  This process is split into two parts: The
	  // part that comprises this method is application-independent. The
	  // remainder is relegated to the Paint() method. The WindProc()
	  // function further responds to the WM_ERASEBKGND by not erasing the
	  // background. We start by creating the device context for handling
	  // the WM_PAINT message.
	CRect rc = GetClientRect();
	// create a memory device context to print into
	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);
	  // select the window font
 	dcMem.SelectObject(m_font);
	  // Create a bitmap big enough for our client rectangle.
	dcMem.CreateCompatibleBitmap(dc, rc.right, rc.bottom);
	CBrush brBkGnd(m_rgbBkColor);
	dcMem.FillRect(rc, brBkGnd);
	dcMem.FillRect(rc, brBkGnd);
	  // set default text foreground and  background colors
	dcMem.SetTextColor(m_rgbTxColor);
	dcMem.SetBkColor  (m_rgbBkColor);
	  // now paint the client rc
	Paint(dcMem, rc);
	  // now we can rapidly copy the painted memory into the screen DC.
	dc.BitBlt(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
	    dcMem, 0, 0, SRCCOPY);
}

/*============================================================================*/
	void CView::
OnInitialUpdate()                                                       /*

	Prepare for the initial view. This method executes immediately after
	the window is created. Recover (deserialize) font and  screen colors.
	If deserialization fails, inform the user and  proceed to use default
	values.
*-----------------------------------------------------------------------------*/
{
	// TODO: add code here  to prepare the initial view

	TRACE("CView window created\n");
}

/*============================================================================*/
	void CView::
SyncScrollBars()                                                        /*

	On opening a new document, align the scroll bar positions in the new
	window with those of the old window.  This is really only needed when
	opening a document at start up time using saved scroll bar positions.
*-----------------------------------------------------------------------------*/
{
	CPoint pt = m_sb.GetScrollPosition();
	m_sb.SetScrollPosition(pt);
}

/*============================================================================*/
     	void    CView::
Paint(CDC &dcMem, RECT rc)                                    		/*

	Paint the view, whose device context is dcMem and whose extent is
	specified by rc, with the top-left point of the screen at the current
	document position. When scrolling is enabled, this point is
	scroll-dependent; if it is not enabled, this point is the first
	character of the document.
*-----------------------------------------------------------------------------*/
{
	// TODO: Enter the code here necessary to display the application

	  // if the font hasn't been instantiated yet, just load the default
	if (m_cWd == 0 || m_cHt == 0)
	{	SetDefaultFont();
		return;
	}
	  // get the document size and  max width
	int doc_length = TheDoc().GetDocLength(),
	    doc_width  = TheDoc().GetDocWidth();
	  // express screen width columns and  height lines in point form
	CPoint 	wh = GetClientWdHt();
	  // display the view content
	CString s;
	if (doc_length > 0 && doc_width > 0)
	{
		  // compute how many lines & cols will appear on the screen
		int lines = MIN(doc_length, wh.y),
		    cols  = MIN(doc_width,  wh.x);

		  // show/hide the scrollbars
		m_sb.ShowHScrollBar(doc_width  > wh.x);
		m_sb.ShowVScrollBar(doc_length > wh.y);
		  // set scroll limits to columns and  lines in the client area
		ScrollLimits sLim(0, doc_width, 0, doc_length);
		m_sb.SetScrollLimits(sLim);
		ScrollIncrements si(1, wh.x, 1, wh.y);
		m_sb.SetScrollIncrements(si);
		  // topleft is the upper-left-most character in the display
		CPoint topleft = m_sb.GetScrollPosition();
		int topline = MAX(topleft.y, 0),
		    leftcol = MAX(topleft.x, 0);
		for (int i = 0; i < lines; i++)
		{
			int j = i + topline;
			if (j >= doc_length)
				continue;

			s =  TheDoc().GetDocRecord(j, leftcol, cols);
			TextOutHi(dcMem, rc, i, s, 0, cols);
		}
	}
	else
	{
		m_sb.ShowHScrollBar(FALSE);
		m_sb.ShowVScrollBar(FALSE);
		  // There is no document, so display a message saying so
		s = _T("No application is present.");
		TextOutHi(dcMem, rc, 5, s, 5, wh.x);
	}
}

/*============================================================================*/
	void CView::
PreCreate(CREATESTRUCT& cs)                                             /*

	Set defaults used by the create function for the view window for
	precise control over the window created.
*-----------------------------------------------------------------------------*/
{
	  // a child window of the main frame, with h- and  v-scrolling
	cs.style = WS_CHILD
	    | WS_HSCROLL | WS_VSCROLL
	    ;
	  // Set the extended style: a 3-D look with border and  sunken edge
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

/*============================================================================*/
	void CView::
PreRegisterClass(WNDCLASS& wc)                                          /*

	Set Window class parameters for precise control over the characteristics
	of the window class created.
*-----------------------------------------------------------------------------*/
{
	  // Set the Window Class name
	wc.lpszClassName = _T("CView");
	  // Set a background brush to white
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	  // Set the default cursor
	wc.hCursor =::LoadCursor(NULL, IDC_ARROW);
	  // Set the class style (not to be confused with the window styles
	  // set in PreCreate)
	wc.style = CS_DBLCLKS;	// Generate left button double click messages
}

/*============================================================================*/
	BOOL CView::
PreTranslateMessage(MSG& Msg)						/*

	Used by CWinApp to translate window messages before they are dispatched
	to theTranslateMessage and  DispatchMessage Windows functions in the
	message loop. MSG contains the message to process. Return a nonzero
	if the message was translated and  should not be dispatched; return
	0 if the message was not translated and  should be dispatched.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(Msg);

//	HWND   hwnd	= Msg.hwnd;  
//	UINT   message	= Msg.message;  
//	WPARAM wParam	= Msg.wParam;  
//	LPARAM lParam	= Msg.lParam;  
//	DWORD  time	= Msg.time;  
//	CPoint  pt	= Msg.pt;  

	  // return 0 if the message was not handled here
	return 0;
}

/*=============================================================================*/
	/* static */ COLORREF CView::
RGBHighlight(COLORREF rgb)                                              /*

	Create the text foreground or background highlight color by
	"inverting" the given color rgb.
*-----------------------------------------------------------------------------*/
{
        return RGB(255 - GetRValue(rgb), 255 - GetGValue(rgb),
	    255 - GetBValue(rgb));
}

/*============================================================================*/
	void CView::
SaveFontSize()                                                           /*

	Record the current font width and  height as view object data
	m_cWd and  m_cHt.
*-----------------------------------------------------------------------------*/
{
	  // get the handle to the view's device context
	CClientDC dc(*this);
	  // select the current font into the device context: save the old
	CFont fold = (HFONT)dc.SelectObject(m_font);
	  // measure the font width and  height
	TEXTMETRIC tm;
	dc.GetTextMetrics(tm);
	m_cWd = tm.tmAveCharWidth;
	m_cHt = tm.tmHeight + tm.tmExternalLeading;
          // restore entry environment
	dc.SelectObject(fold);
}

/*============================================================================*/
	LRESULT CView::
ScrollMessages(UINT uMsg, WPARAM wParam, LPARAM lParam)                 /*

	Handle the scrolling messages from the message loop, including mouse
	wheel rotations and  navigation key actuations. Return TRUE if the
	message was handled, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	return m_sb.ScrollProc(uMsg, wParam, lParam);
}

/*============================================================================*/
        void CView::
Serialize(CArchive &ar)                                               	/*

        Called to serialize or deserialize the view to and  from the archive ar,
        depending on the sense of IsStoring().  Scrollbar parameters are
        considered part of the view, and  are archived here. I
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
            // each item serialized is written to the archive
            // file as a TCHAR stream of the proper length,
            // preceded by that length. In some cases, such
            // as for fonts and  window extents, secondary data
            // are saved from which the primary items are then
            // reconstructed.

              // save m_hfFont
            LOGFONT lf;
        	m_font.GetObject(sizeof(LOGFONT), &lf);
	 		ArchiveObject f(&lf, sizeof(LOGFONT));
        	ar << f;

			  // save font height and  width
			ar << m_cHt;
			ar << m_cWd;

			  // save m_rgbTxColor
			ar << m_rgbTxColor;

			  // save m_rgbBkColor
			ar << m_rgbBkColor;
			
			  // save the scrollbar parameters
			ar << m_sb;
		}
        else    // recovering
        {
            // each item deserialized from the archive is
            // retrieved by first reading its length and  then
            // loading in that number of bytes into the data
            // item saved in the archive, as above. Some items,
            // such as fonts and  screen extents, require
            // additional converstion procedures, as shown below.

              // recover m_hfFont
            LOGFONT lf;
	 		ArchiveObject f(&lf, sizeof(LOGFONT));
        	ar >> f;
			
			  // recover font height and  width
			UINT cHt, cWd;
			ar >> cHt;
  			ar >> cWd;
			
			  // recover view colors
			COLORREF rgbTxColor, rgbBkColor;
			ar >> rgbTxColor;
			ar >> rgbBkColor;

		  // no exception having been raised, set the view parameters
        	m_font.CreateFontIndirect(lf);
			m_rgbTxColor = rgbTxColor;
			m_rgbBkColor = rgbBkColor;
			m_cHt = cHt;
			m_cWd = cWd;
			  // recover scroll bar parameters
			ar >> m_sb;
      	}
}

/*============================================================================*/
	CursorShape CView::
SetCursorShape(CursorShape shape)	             			/*

	Set the view cursor to the new shape and  return the old shape.
*-----------------------------------------------------------------------------*/
{
	CursorShape rtn = m_cursor_shape;
	m_cursor_shape = shape;
	m_hCursor = ::LoadCursor(0,
	    shape == arrow ? IDC_ARROW :
	    shape == help  ? IDC_HELP  :
	    shape == cur_wait  ? IDC_WAIT  :
	    /* else */       IDC_NO);
	return rtn;
}

/*============================================================================*/
        void CView::
SetDefaultFont()                                                        /*

        Create the default app font.
*-----------------------------------------------------------------------------*/
{
	  // compute the logical font height for the nDefaultFontSize
	CClientDC dc(*this);
	  // define default size and face
	int nDefaultFontSize     = 10;
	TCHAR lpszFaceDefault[]  = _T("Courier New");
	  // compute the character height
	long lfHeight = -MulDiv(nDefaultFontSize,
	    dc.GetDeviceCaps(LOGPIXELSY), 72);
	  // set default font characteristics
        int nHeight = lfHeight;		// logical height of font
        int nWidth = 0;			// logical average character width
        int nEscapement = 0;		// angle of escapement
        int nOrientation = 0;           // base-line orientation angle
        int fnWeight = FW_REGULAR;	// font weight
        DWORD fdwItalic = 0;            // italic attribute flag
        DWORD fdwUnderline = 0;         // underline attribute flag
        DWORD fdwStrikeOut = 0;         // strikeout attribute flag
        DWORD fdwCharSet = 0;           // character set identifier
        DWORD fdwOutputPrecision = 0;   // output precision
        DWORD fdwClipPrecision = 0;     // clipping precision
        DWORD fdwQuality = 0;           // output quality
        DWORD fdwPitchAndFamily = 0;    // pitch and  family
        LPCTSTR lpszFace = lpszFaceDefault; // pointer to typeface name string

	  // delete the existing font, if it exists
	if ((HFONT)m_font)
		m_font.DeleteObject();
	  // create the default font
	if (!m_font.CreateFont(nHeight, nWidth, nEscapement, nOrientation,
            fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet,
            fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily,
            lpszFace))
		::MessageBox(NULL, _T("Font creation error."), _T("Error"),
 		    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
	SaveFontSize();
}

/*============================================================================*/
     	void    CView::
TextOutHi(CDC &dc, RECT rc, int line, const CString &s, int leftcol,
    int cols)               						/*

	Output the string s beginning at leftcol for cols chars to the given
	line of the client area with device context dc, bounded by rc.
*-----------------------------------------------------------------------------*/
{
	CString slim = s.Left(cols);
	UINT textalign = dc.GetTextAlign();
	dc.SetTextAlign(TA_NOUPDATECP);
	dc.TextOut((rc.left + m_cWd * leftcol),
	    (rc.top + line * m_cHt), slim.c_str(), s.GetLength());
	dc.SetTextAlign(textalign);
}

/*============================================================================*/
	LRESULT CView::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

	All messages for this window pass through this WndProc. In particular,
	handling scrollbar messages are dispatched here.
*-----------------------------------------------------------------------------*/
{
	switch (uMsg)
	{
	    case WM_SIZE:
		Invalidate();
		break;	// Also do default processing

	    case WM_ERASEBKGND:
		return 1;       // prevents flicker on resizing

	    case WM_LBUTTONDOWN:  // user clicks in the client area
	    {
		  // let the main frame OnCommand() handle this as a
		  // notification from the main window
	    	return TheApp().TheFrame()->SendMessage(WM_COMMAND,
		    (WPARAM)IDW_MAIN, (LPARAM)0);
	    }

	    case WM_SETCURSOR:
	    {
	    	if (!TheApp().TheFrame()->m_help_mode)
			SetCursor(m_hCursor);
		return 1;
	    }
	}

	  // process any scrolling messages
	if (ScrollMessages(uMsg, wParam, lParam) == 0)
		return 0;

	  // pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

