/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: The DDXDemo CMainFrame and NoResizeStatusBar
	class declarations. These classes control the appearance and frame-
	related actions of the DDXDemo program. This program is a modified
	version of that found in the FormDocView sample distributed with the
	Win32++ Windows interface classes, Copyright (c) 2005-2015 David Nash,
	used under permissions granted therein. The modified sample program was
	based on code provided by Lynn Allan. This proram extends the given
	sample by application of Dialog Data Exchange and Validation DDX/DDV).
	The NoResizeStatusBar class creates a status bar without the resizing
	gripper in the lower-right-hand corner.

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

*******************************************************************************/

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "Doc.h"


/*******************************************************************************

	Declaration of the CMainFrame class

*=============================================================================*/
	class
CMainFrame : public CFrame

/*----------------------------------------------------------------------------*/
{
	public:
		CMainFrame(void);
		virtual ~CMainFrame();
		CDoc& TheDoc()   { return m_Doc; }
		CView& TheView() { return m_View; };

		BOOL UpdateDialog(BOOL bReadFromControl = SENDTOCONTROL);

	protected:
		virtual BOOL LoadRegistrySettings(LPCTSTR szKeyName);
		virtual void OnClose();					
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual int  OnCreate(CREATESTRUCT& cs);  
		virtual void OnFileExit();
		virtual void OnInitialUpdate();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetReBarColors(COLORREF clrBkGnd1,
				COLORREF clrBkGnd2, COLORREF clrBand1,
				COLORREF clrBand2);
		virtual BOOL SetThemeColors();
		virtual void SetupToolBar();
		virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

	private:
		CView	m_View;
		CDoc	m_Doc;
		std::vector<UINT> m_vBandIDs;
		std::vector<UINT> m_vBandStyles;
		std::vector<UINT> m_vBandSizes;
};

/*============================================================================*/

  // declare access to the CDoc class to be global in scope
CDoc& TheDoc();

/*----------------------------------------------------------------------------*/
#endif //MAINFRM_H

