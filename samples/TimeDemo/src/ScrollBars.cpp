/* (19-May-2014) [Tab/Indent: 8/8][Line/Box: 80/74]           (ScrollBars.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Content Description: WScrollBars class implementation file. This
	class provides scroll bar support to CWnd objects (not dialog forms)
	that have declared scroll bars in the creation of these windows. This
	class uses the Win32++ Windows interface classes, Copyright (c)
	2005-2015 David Nash, under permissions granted therein.

	This class has been adapted to use parts of the Win32++ Windows
	interface classes, Copyright (c) 2005-2015 David Nash, under
	permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
        treatment of material that may have been extracted from online uncited
	sources. Unaltered portions of those materials retain their original
	copyright status. The author's additions and  modifications may be used
	 or  altered by others for non-commercial use only without permission.
	However, citation of the author's work should be included in such
	usages.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the WScrollBars class

	Implementation and  Usage Notes: It is presumed that a CWnd-derived
	view has been created having both horizontal and  vertical scroll bars
	by specifying the WM_HSCROLL and  WM_HSCROLL options, and  that this
	view then handles the limits, increments, and  positioning of each. It
	is necessary to declare a WScrollBars object private, if possible) in
	the CWnd-derived class declaration, as, for example,

		WScrollBars m_sb;         // scroll bars interface

	Then, in the OnInitialUpdate() method of the view, register the view
	with the scrolling object, as, for example,

	  // attach scroll bars to this view
	m_sb.Register(this);
	  // hide scroll bars for now
	m_sb.ShowHScrollBar(FALSE);
	m_sb.ShowVScrollBar(FALSE);

	Each scroll bar position is assumed to range between certain minimum
	 and  maximum values, expressed in scroll units. At the minimum value,
	the position is at the top or left scroll limit, and  if at the maximum
	value, the thumb is at the bottom or right scroll limit. Scrolling may
	be initiated by mouse clicks on the bars, by dragging the "thumb" with
	the mouse, or by rotating the mouse wheel.

	Users may change the scrolling limits, increments, and  position, and
	they may access the current values of limits, increments, and  position.
	It is up to the user then to draw the application client area in
	accordance with these limits, page sizes, increments, and  positions.

*******************************************************************************/

#include "ScrollBars.h"

/*******************************************************************************

	Implementation of ScrollLimits class

*=============================================================================*/
	void ScrollLimits::
Serialize(CArchive &ar)							/*

        Serialize or deserialize the scroll limits to and  from the archive ar,
	depending on the sense of IsStoring().
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
		ar << m_h_min;
		ar << m_h_max;
		ar << m_v_min;
		ar << m_v_max;
	}
        else    // recovering
        {
		ar >> m_h_min;
		ar >> m_h_max;
		ar >> m_v_min;
		ar >> m_v_max;
      	}
}

/*******************************************************************************

	Implementation of ScrollIncrements class

*=============================================================================*/
	void ScrollIncrements::
Serialize(CArchive &ar)							/*

        Serialize or deserialize the scroll increments to and  from the 
	archive ar, depending on the sense of IsStoring().
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
		ar << m_h_line;
		ar << m_h_page;
		ar << m_v_line;
		ar << m_v_page;
	}
        else    // recovering
        {
 		ar >> m_h_line;
		ar >> m_h_page;
		ar >> m_v_line;
		ar >> m_v_page;
     	}
}

/*******************************************************************************



*=============================================================================*/
	WScrollBars::
WScrollBars()                                                            /*

	Construct WScrollBars object. This object implements scroll bars for
	CWnd views that have been created with WM_HSCROLL and  WM_HSCROLL
	options enabled.
*-----------------------------------------------------------------------------*/
{
	m_theView = NULL;
}

/*============================================================================*/
	WScrollBars::
~WScrollBars()                                                           /*

	Destruct WScrollBars object.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	ScrollIncrements WScrollBars::
GetScrollIncrements(void)						/*

	Return the current scrolling increments, in scroll units.
*-----------------------------------------------------------------------------*/
{
	return m_increments;
}

/*============================================================================*/
	ScrollLimits WScrollBars::
GetScrollLimits(void)							/*

	Return the current scrolling limits, in scroll units.
*-----------------------------------------------------------------------------*/
{
	return m_limits;
}

/*============================================================================*/
	CPoint WScrollBars::
GetScrollPosition(void)							/*

	Return the current scroll position coordinates, in scroll units and
	set the m_scroll_position member to this value.
*-----------------------------------------------------------------------------*/
{
	if (!m_theView)
		return CPoint(0, 0);

	int x, y;
	  // Get the current vertical scroll position from the CView window
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask  = SIF_POS;
	m_theView->GetScrollInfo(SB_VERT, si);
	y = si.nPos;
	  // Get the current horizontal scroll position
	m_theView->GetScrollInfo(SB_HORZ, si);
	x = si.nPos;
	m_scroll_position = CPoint(x, y);
	return m_scroll_position;
}

/*============================================================================*/
        LRESULT WScrollBars::
OnHScroll(WPARAM wParam, LPARAM lParam)      	 			/*

        Processes horizontal scrolling requests. The scroll code is given in
	LOWORD(wParam).  HIWORD(wParam) specifies the current position of
	the scroll box if the scroll code is SB_THUMBPOSITION or SB_THUMBTRACK;
	but here it is not used. The lParam is the handle to the scroll bar
	control if the message was sent by the scroll bar; otherwise, it is
	NULL. Thererfore, since we have messages coming from the keyboard
	as well as the scrollbar, only the scroll code is used here. The
	function returns 0.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);
	if (!m_hscroll_visible)
		return 0;

	if (!m_theView)
		return 0;

	  // Get the current horizontal scroll bar information from the CView
	  // window
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask  = SIF_ALL;
	m_theView->GetScrollInfo(SB_HORZ, si);
	  // respond to the scroll event:
	UINT nPos = si.nPos;
	switch (LOWORD(wParam)) // scroll command
	{
	    case SB_LEFT:		//   Scroll to far left.
		nPos = m_limits.m_h_min;
		break;

	    case SB_RIGHT:		//   Scroll to far right.
		nPos = m_limits.m_h_max;
		break;

	    case SB_LINELEFT:           // scroll left one column-unit
		nPos -= m_increments.m_h_line;
		break;

	    case SB_LINERIGHT:           // scroll right one column-unit
		nPos += m_increments.m_h_line;
		break;

	    case SB_PAGELEFT:           // scroll left one column-page-unit
		nPos -= m_increments.m_h_page;
		break;

	    case SB_PAGERIGHT:           // scroll right one column-page-unit
		nPos += m_increments.m_h_page;
		break;

	    case SB_THUMBTRACK:		// h-thumb is moving
	    case SB_THUMBPOSITION:      // or has stopped
		nPos = si.nTrackPos; // was HIWORD(wParam);
		break;

	default:	// the rest are immaterial
		break;
	}
 	  // Reset new horizontal scroll position (automatically puts
	  // m_state.nPos within the specified limits).
	si.fMask = SIF_POS;
	si.nPos  = nPos;
	m_theView->SetScrollInfo(SB_HORZ, si, TRUE);
	m_theView->Invalidate(FALSE);
	return 0;
}

/*============================================================================*/
        LRESULT WScrollBars::
OnKeyScroll(WPARAM wParam, LPARAM lParam)        			/*

	Processes nonsystem (i.e., non ALT+key) keystrokes: wParam contains
	the virtual key code. lParam combines repeat count, OEM scan code,
	extended-key flag, and  previous key-state flag.  Only the virtual
	key code is used here.

	The usual scrolling functions of keys are implemented here.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	if (!m_theView)
		return 0;

	UINT 	wScrollNotify = 0,
		uMessage = 0;

	  // get the state of the control key
	bool control = ((::GetKeyState(VK_CONTROL) & 0x8000) ? true : false);
	  // respond to the scroll event:
	switch (wParam)
	{
            case VK_HOME:       // HOME key
		if (control)
		{         // top limit of scroll
		        wScrollNotify = SB_TOP;
		        uMessage = WM_VSCROLL;
        		m_theView->SendMessage(uMessage,
			    MAKELONG(wScrollNotify, 0), 0L);
		        wScrollNotify = SB_LEFT;
		        uMessage = WM_HSCROLL;
		}
		else
		{         // left limit of scroll
		        wScrollNotify = SB_LEFT;
		        uMessage = WM_HSCROLL;
		}
	        break;

	    case VK_END:        // END key
		if (control)
		{         // bottom limit of scroll
		        wScrollNotify = SB_BOTTOM;
		        uMessage = WM_VSCROLL;
        		m_theView->SendMessage(uMessage,
			    MAKELONG(wScrollNotify, 0), 0L);
		        wScrollNotify = SB_LEFT;
		        uMessage = WM_HSCROLL;
		}
		else
		{         // right limit of scroll
		        wScrollNotify = SB_RIGHT;
		        uMessage = WM_HSCROLL;
		}
	        break;

	    case VK_PRIOR:    	//PAGEUP key
	        wScrollNotify = SB_PAGEUP;
	        uMessage = WM_VSCROLL;
	        break;

	    case VK_NEXT:     	// PAGEDOWN key
	        wScrollNotify = SB_PAGEDOWN;
	        uMessage = WM_VSCROLL;
	        break;

	    case VK_UP:         // UPARROW key
		if (control)
		{         // one vertical increment up
			wScrollNotify = SB_LINEUP;
		        uMessage = WM_VSCROLL;
	        break;
		}

	    case VK_LEFT:       // LEFTARROW key
		if (control)
		{         // one column increment left
		        wScrollNotify = SB_LINELEFT;
		        uMessage = WM_HSCROLL;
	        break;
		}

	    case VK_RIGHT:      // RIGHTARROW key
		if (control)
		{         // one column increment right
		        wScrollNotify = SB_LINERIGHT;
		        uMessage = WM_HSCROLL;
	        break;
		}

	    case VK_DOWN:       // DOWNARROW key
		if (control)
		{         // one vertical increment down
		        wScrollNotify = SB_LINEDOWN;
		        uMessage = WM_VSCROLL;
	        }
	        break;

	    default:
	        wScrollNotify = 0xFFFF;
	        break;
	}
	  // send the keystroke message
	if (wScrollNotify != 0xFFFF)
		::SendMessage(m_theView->GetHwnd(), uMessage,
		    MAKELONG(wScrollNotify, 0), 0L);

	return 0;
}

/*============================================================================*/
        LRESULT WScrollBars::
OnVScroll(WPARAM wParam, LPARAM lParam)        				/*

        This function processes vertical scrolling requests. The scroll code
	is given in LOWORD(wParam). HIWORD(wParam) specifies the current
	scroll box if the scroll code is SB_THUMBPOSITION or SB_THUMBTRACK;
	but here it is not used. lParam is the handle to the scroll bar
	control if the message was sent by the scroll bar; otherwise,
	it is NULL. Thererfore, since we have messages coming from the
	keyboard as well as the scrollbar, only the scroll code is used here.
	The function returns 0.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	if (!m_vscroll_visible)
		return 0;

	  // Get the current vertical scroll bar information
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask  = SIF_ALL;
	m_theView->GetScrollInfo(SB_VERT, si);
	  // respond to the scroll event:
	UINT nPos = si.nPos;
	switch (LOWORD(wParam))  // the scroll code
	{
	    case SB_TOP:                // top of scroll
		nPos = m_limits.m_v_min;
		break;

	    case SB_BOTTOM:             // bottom of scroll
		nPos = m_limits.m_v_max;
		break;

	    case SB_LINEUP:             // up one line-unit
		nPos -= m_increments.m_v_line;
		break;

	    case SB_LINEDOWN:           // down one line-unit
		nPos += m_increments.m_v_line;
		break;

	    case SB_PAGEUP:             // up one page-unit
		nPos -= m_increments.m_v_page;
		break;

	    case SB_PAGEDOWN:           // down one page-unit
		nPos += m_increments.m_v_page;
		break;

	    case SB_THUMBTRACK:         // the thumb is moving
	    case SB_THUMBPOSITION:      // or has stopped
		nPos = si.nTrackPos; // was HIWORD(wParam);
		break;

	    default:
		break;
	}
	  // Reset new vertical scroll position (automatically puts
	  // nPos within the specified limits).
	si.fMask = SIF_POS;
	si.nPos  = nPos;
	m_theView->SetScrollInfo(SB_VERT, si, TRUE);
	m_theView->Invalidate(FALSE);
	return 0;
}

/*============================================================================*/
        LRESULT WScrollBars::
OnMouseWheel(WPARAM wParam, LPARAM lParam)        			/*

        Processes mouse wheel rotation when made in the focus window having
	a vertical scrollbar.  LOWORD(wParam) contains the key flags,
	HIWORD(wParam) gives the (signed) wheel rotation, GET_X_PARAM(lParam)
	is the horizontal position of the pointer, and  GET_Y_PARAM(lParam)
	is the vertical position of pointer.  The flags and  position parameters
	are unused here.  The incoming rotation parameter will be a multiple
	of the WHEEL_DELTA value, which is set at 120. Each such unit here
	scrolls one virtical increment. The function returns 0.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	if (!m_hscroll_visible)
		return 0;

	  // get all current vertical scroll info
	SCROLLINFO si;
        si.cbSize = sizeof (si);
        si.fMask  = SIF_ALL;
        m_theView->GetScrollInfo(SB_VERT, si);
	  // set the new position: first get (signed) rotation units
	int rotation = HIWORD(wParam);
	  // convert to line increments, maintaining sign
	rotation = (rotation * m_increments.m_v_line) / WHEEL_DELTA;
	si.nPos += rotation;

	// reset new vertical scroll position
	si.fMask = SIF_POS;
	m_theView->SetScrollInfo(SB_VERT, si, TRUE);
	m_theView->Invalidate(FALSE);
	return 0;
}

/*============================================================================*/
        void WScrollBars::
Register(CWnd *theView)							/*

        Initialize scroll bars already attached to theView.  This member
        cannot be called until the window has been created. the
	OnInitialUpdate() of the view is a good place for this call.
*-----------------------------------------------------------------------------*/
{
	  // attach the view to this object
	m_theView = theView;
 	  // set initial limits
	ScrollLimits sl(0, 100, 0, 100);
	SetScrollLimits(sl);
	  // set initial increments
	ScrollIncrements si(1, 1, 1, 1);
	SetScrollIncrements(si);
	  // hide the scroll bars until needed
        ShowHScrollBar(FALSE);
        ShowVScrollBar(FALSE);
}

/*============================================================================*/
	LRESULT WScrollBars::
ScrollProc(UINT uMsg, WPARAM wParam, LPARAM lParam)         		/*

	Process the system message uMsg from the view's message loop. All
	scroll request messages pass through this procedure.  Return 0 if
	the message was handled, -1 otherwise.
*-----------------------------------------------------------------------------*/
{
	switch (uMsg)
	{
	    case WM_VSCROLL: 	// received when the v-scrollbar is clicked
                if (OnVScroll(wParam, lParam) == 0)
                        return 0;

                break;

	    case WM_HSCROLL: 	// received when the v-scrollbar is clicked
                if (OnHScroll(wParam, lParam) == 0)
                        return 0;

                break;

	    case WM_KEYDOWN:  // received when a key is pressed
                if (OnKeyScroll(wParam, lParam) == 0)
                        return 0;

		break;

	    case WM_MOUSEWHEEL:  // rotation detected
		if (OnMouseWheel(wParam, lParam) == 0)
			return 0;

		break;
	}
	  // message was not one for scrolling: notify caller no processing
	  // took place
	return -1;
}

/*============================================================================*/
        void WScrollBars::
Serialize(CArchive &ar)                                               /*

        Serialize or deserialize the toolbars to and  from the archive ar,
	depending on the sense of IsStoring(). 
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
                  // each item serialized is written to the archive
                  // file as a char stream of the proper length,
                  // preceded by that length. In some cases, such
                  // as fonts and  window extents, secondary data
                  // are saved, from with the primary items are then
                  // reconstructed.

                  // save toolbars state
  		ar << m_hscroll_visible;
		ar << m_vscroll_visible;
		ar << m_increments;
		ar << m_limits;
		GetScrollPosition();
		ar << m_scroll_position;
	}
        else    // recovering
        {
                  // each item deserialized from the archive is
                  // retrieved by first reading its length and then
                  // loading in that number of bytes into the data
                  // item saved in the archive, as above. Some items,
                  // such as fonts and  screen extents, require
                  // additional converstion procedures, as shown below.

                  // recover toolbars parameters:
  		ar >> m_hscroll_visible;
		ar >> m_vscroll_visible;
		ar >> m_increments;
		ar >> m_limits;
		ar >> m_scroll_position;
      	}
}

/*============================================================================*/
	void WScrollBars::
SetScrollIncrements(ScrollIncrements &sinc)				/*

	Set the current scrolling increments to those in sinc. The page
	increments are also entered into the SCROLLINFO data for each bar.
*-----------------------------------------------------------------------------*/
{
	if (!m_theView)
		return;

	SCROLLINFO si;
	m_increments = sinc;
	si.cbSize = sizeof (si);
	si.fMask = SIF_PAGE;
	si.nPage = sinc.m_v_page;
	m_theView->SetScrollInfo(SB_VERT, si, TRUE);
	si.nPage = sinc.m_h_page;
	m_theView->SetScrollInfo(SB_HORZ, si, TRUE);
}

/*============================================================================*/
	void WScrollBars::
SetScrollLimits(ScrollLimits &lims)					/*

	Set the current scrolling limits to those in lims.
*-----------------------------------------------------------------------------*/
{
	if (!m_theView)
		return;

	m_limits = lims;
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_RANGE;
	si.nMin = lims.m_v_min;
	si.nMax = lims.m_v_max;
	m_theView->SetScrollInfo(SB_VERT, si, TRUE);
	si.nMin = lims.m_h_min;
	si.nMax = lims.m_h_max;
	m_theView->SetScrollInfo(SB_HORZ, si, TRUE);
}

/*============================================================================*/
	void WScrollBars::
SetScrollPosition(CPoint pt)						/*

	Set the horizontal and  vertical scroll positions, respectively, to
	x and  y, respectively and  redraw the scroll bars.
*-----------------------------------------------------------------------------*/
{
	if (!m_theView)
		return;

	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_POS;
	si.nPos = pt.x;
	m_theView->SetScrollInfo(SB_HORZ, si, TRUE);
	si.nPos = pt.y;
	m_theView->SetScrollInfo(SB_VERT, si, TRUE);
	m_scroll_position = pt;
}

/*============================================================================*/
        void WScrollBars::
ShowHScrollBar(BOOL show)                                         	/*

	Show or hide the horizontal scroll bar, as per the show parameter.
*-----------------------------------------------------------------------------*/
{
	if (!m_theView)
		return;

        m_hscroll_visible = show;
	m_theView->ShowScrollBar(SB_HORZ, m_hscroll_visible);
}

/*============================================================================*/
        void WScrollBars::
ShowVScrollBar(BOOL show)                                         	/*

	Show or hide the vertical scroll bar, as per the show parameter.
*-----------------------------------------------------------------------------*/
{
 	if (!m_theView)
		return;

        m_vscroll_visible = show;
	m_theView->ShowScrollBar(SB_VERT, m_vscroll_visible);
}

