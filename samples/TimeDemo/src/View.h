/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the CView class for a skeleton
	SDI application using the Win32++ Windows interface classes,
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

	Declaration of the CView class

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

/******************************************************************************/

#include "ScrollBars.h"

  // cursor shape alternatives
enum CursorShape {arrow, cur_wait, help};

/*============================================================================*/
	class
CView : public CWnd							/*

	The application's view of its currently open document.
*-----------------------------------------------------------------------------*/
{
	public:
		CView();
		virtual ~CView();

		virtual void	SaveFontSize();
		virtual LRESULT ScrollMessages(UINT, WPARAM, LPARAM);
		virtual	void 	Serialize(CArchive &ar);
		virtual CursorShape SetCursorShape(CursorShape);
		virtual void 	SyncScrollBars();
		
		  // public data members
                CFont     m_font;         // the font used to display view
                COLORREF  m_rgbBkColor,	  // background color
                	  m_rgbBkHiColor, // background highlight color
			  m_rgbCustomColors[16], // custom colors defined
                	  m_rgbTxColor,   // text foreground color
			  m_rgbTxHiColor; // text foreground highlight color

	     	static const CString m_sCompiled_on;  // compilation date

	protected:
		virtual CPoint 	GetClientWdHt();
		virtual int	OnCreate(CREATESTRUCT& cs);  
		virtual void 	OnDraw(CDC& pDC);
		virtual void 	OnInitialUpdate();
		virtual void    Paint(CDC &, RECT);
		virtual void 	PreCreate(CREATESTRUCT& cs);
		virtual void 	PreRegisterClass(WNDCLASS& wc);
		virtual BOOL 	PreTranslateMessage(MSG& Msg);  
		virtual void    SetDefaultFont();
     		virtual void    TextOutHi(CDC&, RECT, int, const CString &,
		     		    int, int);
		virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

		  // protected static method
		static	COLORREF RGBHighlight(COLORREF);

		  // protected data members
       		UINT	    m_cWd,        // font average width
			    m_cHt; 	  // font average height
		WScrollBars m_sb;         // scroll bars interface
		CursorShape m_cursor_shape;// view arrow/help/wait
		HCURSOR     m_hCursor;     // view cursor

	private:
		  // private methods
};
/*----------------------------------------------------------------------------*/
#endif // SDI_VIEW_H
