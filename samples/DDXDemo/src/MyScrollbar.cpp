/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]          (MyScrollBar.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: The custom scroll bar class implementation for
	this DDX-DDV Test program. This class is derived from the MyScrollBar
	class found in the Win32++ Windows sample programs, Copyright
	(c)2005-2015 David Nash, under permissions granted therein.

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

	Implementation of the CMyScrollBar class

*******************************************************************************/

#include "stdafx.h"
#include "MyScrollbar.h"
#include "resource.h"
#include "App.h"

/*============================================================================*/
	CMyScrollBar::
CMyScrollBar()                                                           /*

*-----------------------------------------------------------------------------*/
{
	ZeroMemory(&m_siScroll, sizeof(SCROLLINFO));
}

/*============================================================================*/
	LRESULT CMyScrollBar::
OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam) 			/*

*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(lParam);

	GetScrollInfo(m_siScroll);

	switch (LOWORD (wParam))
	{
	    case SB_LINELEFT:  // user clicked left or up arrow
		m_siScroll.nPos -= 1;
		break;

	    case SB_LINERIGHT:  // clicked right or down arrow
		m_siScroll.nPos += 1;
		break;

	    case SB_PAGELEFT:  // clicked page up or scroll bar left of thumb
		m_siScroll.nPos -= m_siScroll.nPage;
		break;

	    case SB_PAGERIGHT: // clicked page down or scroll bar right of thumb
		m_siScroll.nPos += m_siScroll.nPage;
		break;

	    case SB_THUMBTRACK: // user dragged the scroll box
		m_siScroll.nPos = m_siScroll.nTrackPos;
		break;

	    default :
		break;
	}

	  // get a view dialog reference
	CView& theView = TheApp().TheFrame()->TheView();
	  // get the scoll bar position, set slider and progress to same
	theView.m_iSlider = theView.m_iScrollBar =
	    theView.m_iProgress = m_siScroll.nPos;
	theView.UpdateDialog(SENDTOCONTROL);
	theView.m_nIDFocus = IDC_SCROLLBAR;
	theView.AdjustParameters();

	return 0L;
}

/*============================================================================*/
	LRESULT CMyScrollBar::
OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)		/*

*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
	    case WM_HSCROLL:
		return OnHScroll(uMsg, wParam, lParam);
	}

	return 0L;
}

/*============================================================================*/
	void CMyScrollBar::
SetScrollInfo(int lo, int hi, int pos, int page)			/*

*-----------------------------------------------------------------------------*/
{
	m_siScroll.cbSize = sizeof(SCROLLINFO);
	m_siScroll.nPos   = pos;
	m_siScroll.nPage  = page;
	m_siScroll.nMin   = lo;
	m_siScroll.nMax   = hi + (page - 1);
	m_siScroll.fMask  = SIF_ALL;

	  // Set the scroll bar position
	CScrollBar::SetScrollInfo(m_siScroll, TRUE);
}

