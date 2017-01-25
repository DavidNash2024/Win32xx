/* (02-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a basic CMainFrame class for the
	ScrollWin demo application using the Win32++ Windows interface classes,
	Copyright (c) 2005-2017 David Nash, under permissions granted therein.
	
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

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/******************************************************************************/


  // designation of where a control id appears, used when enabling/disabling
enum ControlBars {toolbar, mainmenu, both};

/*============================================================================*/
	class 
CMainFrame : public CFrame						/*

*-----------------------------------------------------------------------------*/
{
	public:
		CMainFrame(void);
		virtual 	~CMainFrame();
		CDoc& 		TheDoc() {return m_Doc;}
		virtual void	Serialize(CArchive& ar);

	  // public data members
	     	static  CString	m_sCompiled_on; // compilation date, mmm dd yyyy

	protected:
        	virtual void   	OnColorChoice(void);
		virtual BOOL 	OnCommand(WPARAM wParam, LPARAM lParam);
		virtual int  	OnCreate(CREATESTRUCT& cs);
		virtual void 	OnFileClose(void);
		virtual void 	OnFileExit(void);
		virtual void 	OnFileOpenMRU(UINT);
		virtual void 	OnFileOpen(void);
        	virtual void   	OnFontChoice(void) { m_View.ClientFontChoice();}
		virtual BOOL	OnHelp(void);
		virtual void 	OnInitialUpdate(void);
		virtual void 	OnMenuUpdate(UINT nID);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual BOOL	OnProcessMRU(WPARAM wParam, LPARAM lParam);
		virtual void 	PreCreate(CREATESTRUCT& cs);
		virtual BOOL 	PreTranslateMessage(MSG &Msg);
		virtual BOOL 	SaveRegistrySettings(void);
		virtual void    SetupToolBar(void);
		virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

		  // protected data members
		CDoc 	   m_Doc;	   // the document
		CView 	   m_View;	   // the view
                UINT 	   m_win_x,        // deserialized window x position
                           m_win_y,        // deserialized window y position
                           m_win_width,    // deserialized window width
                           m_win_height;   // deserialized window height
		UINT       m_nMaxMRU;      // limit on MRU entries
		AboutBox   m_AboutDialog;  // about dialog handled here
		WINDOWPLACEMENT m_Wndpl;   // window placement information

	private:

};

/*==============================================================================

	Declare the global function that provides access to the
	CDoc class 							*/
	
CDoc& TheDoc();

/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

