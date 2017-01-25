/* (14-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the CScrollWnd demo application
	view class for a typical Windows application using the Win32++ Windows
	interface classes, Copyright (c) 2005-2017 David Nash, under 
	permissions granted therein.

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

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Network Planning
		and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CAPP_VIEW_H
#define CAPP_VIEW_H

#include "FontEx.h"

/*============================================================================*/
	class
CColorExDialog : public CColorDialog                                      /*

	Extend the CColorDialog class to change the window title.
*-----------------------------------------------------------------------------*/
{
	public:
		CColorExDialog(COLORREF clrInit = 0, DWORD dwFlags = 0)
		    : CColorDialog(clrInit, dwFlags | CC_ENABLEHOOK |
			CC_RGBINIT | CC_ANYCOLOR)
		{
			SetBoxTitle(_T("Color..."));
		}

		~CColorExDialog(){}

		void	SetBoxTitle(const CString& title)
			    { m_sBoxTitle = title;}

	protected:
		virtual BOOL 	OnInitDialog()
				    {
					CColorDialog::OnInitDialog();
					SetWindowText(m_sBoxTitle);
					return TRUE;
				    }

	private:
		CString 	m_sBoxTitle;
};


/*============================================================================*/
	class
CView : public CScrollWnd                                              /*

	The application scrolling window class.
*-----------------------------------------------------------------------------*/
{
	public:
		CView();
		virtual ~CView();

		virtual CPoint	DevToScrl(CPoint) const;
		virtual void	ClientFontChoice();
		virtual void    OnColorChoice();
		virtual CPoint	ScrlToDev(CPoint) const;
		virtual	void 	Serialize(CArchive &ar);
		virtual void 	SetNewAppSize();

	protected:
		virtual	void	InitCustomColors();		
		virtual void 	OnInitialUpdate();
		virtual void    Paint(CDC& dcMem);
		virtual void 	PreCreate(CREATESTRUCT &cs);
		virtual void 	PreRegisterClass(WNDCLASS &wc);
		virtual BOOL 	PreTranslateMessage(MSG &Msg);
		virtual void    SetScrollAppInfo();
     		virtual void    TextLineOut(CDC&, UINT, UINT, CString);
		virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

	private:
		  // private data members
                CFontEx		m_VuFont;	        // the view display font
                COLORREF 	m_rgbCustomColors[16];	// custom colors defined
		CSize		m_doc_extent;		// the document extent
};
/*----------------------------------------------------------------------------*/
#endif  //CAPP_VIEW_H
