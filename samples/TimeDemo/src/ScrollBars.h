/* (19-May-2014) [Tab/Indent: 8/8][Line/Box: 80/74]             (ScrollBars.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Content Description: WScrollBars class declaration file. This
	class provides scroll bar support to CWnd objects (not dialog forms)
	that have declared scroll bars in the creation of these windows. This
	class uses the Win32++ Windows interface classes, Copyright (c)
	2005-2015 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from cited sources. Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, together with the respective
		permission conditions shall be included in all copies  or
		substantial portions of this material so copied, modified,
		merged, published, distributed, or otherwise held by others.

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

	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		
		
********************************************************************************

	Declarations of the WScrollBars, ScrollLimits, and  ScrollIncrements
	classes

*******************************************************************************/

#ifndef WScrollBars_H_DEFINED
#define WScrollBars_H_DEFINED


/*============================================================================*/
	class
ScrollLimits : public CObject    					/*

	A class that holds all scroll bar limit values and the means to
	archive them.
*-----------------------------------------------------------------------------*/
{
	public:
		ScrollLimits()
		    {m_h_min = m_h_max = m_v_min = m_v_max = 0;}
		ScrollLimits(UINT a, UINT b, UINT c, UINT d)
		    {m_h_min = a; m_h_max = b; m_v_min = c; m_v_max = d;}

		virtual	void Serialize(CArchive &ar);

		  // public member data
		UINT m_h_min;
		UINT m_h_max;
		UINT m_v_min;
		UINT m_v_max;
};

/*============================================================================*/
	class
ScrollIncrements : public CObject                          		/*

	A class that holds the line and page increments and the means to
	archive them.
*-----------------------------------------------------------------------------*/
{
	public:
		ScrollIncrements()
		    {m_h_line = m_h_page = m_v_line = m_v_page = 0;}
		ScrollIncrements(UINT a, UINT b, UINT c, UINT d)
		    {m_h_line = a; m_h_page = b; m_v_line = c; m_v_page = d;}

	virtual	void Serialize(CArchive &ar);

		  // public member data
		UINT m_h_line;
		UINT m_h_page;
		UINT m_v_line;
		UINT m_v_page;
};

/*============================================================================*/
	class
WScrollBars : public CObject     					/*

	This class provides scroll bar support to CWnd objects that have already
	declared scroll bars in the creation of its windows. This class assumes
	both vertical and  horizontal bars, and  is not derived from either CWnd
	 or  CScrollBar. However, it does invoke CWnd methods that provide the
	scrolling functions.
*-----------------------------------------------------------------------------*/
{
	public:

	  // construction and  destruction:
	WScrollBars();
	~WScrollBars();

	  // public method members
	ScrollIncrements GetScrollIncrements(void);
	ScrollLimits 	 GetScrollLimits(void);
	CPoint		 GetScrollPosition(void);
	LRESULT 	 OnHScroll(WPARAM, LPARAM);
	LRESULT 	 OnKeyScroll(WPARAM, LPARAM);
	LRESULT 	 OnMouseWheel(WPARAM, LPARAM);
	LRESULT 	 OnVScroll(WPARAM, LPARAM);
	void    	 Register(CWnd *);
	LRESULT 	 ScrollProc(UINT, WPARAM, LPARAM);
	virtual	void 	 Serialize(CArchive &ar);
	void    	 SetScrollIncrements(ScrollIncrements&);
	void    	 SetScrollLimits(ScrollLimits&);
	void    	 SetScrollPosition(CPoint pt);
	void    	 ShowHScrollBar(BOOL show);
	void    	 ShowVScrollBar(BOOL show);

	protected:

	private:

	  // private data members
	CPoint           m_scroll_position;     // (h, v) position
	BOOL    	 m_hscroll_visible;     // h scroll is visible
	ScrollIncrements m_increments;          // scroll increments
	ScrollLimits     m_limits;              // scroll limits
	CWnd            *m_theView; 		// the view window
	BOOL    	 m_vscroll_visible;     // v scroll is visible
};

/*----------------------------------------------------------------------------*/
#endif // WScrollBars_H_DEFINED
