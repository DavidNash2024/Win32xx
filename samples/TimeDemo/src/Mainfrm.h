/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a CMainFrame class for a
	skeleton SDI application using the Win32++ Windows interface classes,
	Copyright (c) 2005-2015 David Nash, under permissions granted therein.
	
        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any persion obtaining a copy of this treatment
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

	Declaration of the CMainFrame class

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/******************************************************************************/

#include "wxx_frame.h"

#include "View.h"
#include "Doc.h"
#include "AboutBox.h"
#include "MRU.h"

  // designation of where a control id appears, used when enabling/disabling
enum ControlBars {toolbar, mainmenu, both};

/*============================================================================*/
	class 
CMainFrame : public CFrame						/*

	The application mainframe class that creates the main menu, toolbar,
	status bar, and CView display, and responds to user interactions.
*-----------------------------------------------------------------------------*/
{
	public:
				CMainFrame();
		virtual 	~CMainFrame();
		
		virtual void    OnUpdateStatus();
		virtual void    SetCursorShape(CursorShape);
		virtual void	SetWindowTitle(const CString &title = _T(""));
		CView&          TheView(){return m_View;}
		CDoc& 		ThisDoc(){return m_Doc;}
		CMRU&		TheMRU() {return m_MRU;};

		  // public data members
			bool    m_help_mode;
			
	     	static const CString m_sCompiled_on; // compilation date

	protected:
		virtual void    OnColorChoice();
		virtual BOOL 	OnCommand(WPARAM wParam, LPARAM lParam);
		virtual BOOL 	OnCommandAction(WPARAM wParam, LPARAM lParam);
		virtual BOOL 	OnCommandHelp(WPARAM wParam, LPARAM lParam);
		virtual int  	OnCreate(LPCREATESTRUCT pcs);
		virtual void	OnDestroy();
		virtual void 	OnEditCopy();
		virtual void 	OnEditCut();
		virtual void 	OnEditDelete();
		virtual void 	OnEditFind();
		virtual void 	OnEditPaste();
		virtual void 	OnEditRedo();
		virtual void 	OnEditReplace();
		virtual void 	OnEditUndo();
		virtual void 	OnFileClose();
		virtual void 	OnFileExit();
		virtual void 	OnFileNew();
		virtual void 	OnFileOpen();
		virtual bool 	OnFileOpenMRU(UINT);
		virtual void 	OnFilePrint();
		virtual void 	OnFilePrintPreview();
		virtual void 	OnFileSave();
		virtual void 	OnFileSaveAs();
		virtual void    OnFontChoice();
		virtual BOOL	OnHelp();
		virtual BOOL	OnHelp(const CString&);
		virtual BOOL	OnHelpAbout();
		virtual BOOL	OnHelpContext();
		virtual void 	OnInitialUpdate();
		virtual void 	OnMenuUpdate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual BOOL	OnProcessMRU(WPARAM wParam, LPARAM lParam);
		virtual void 	PreCreate(CREATESTRUCT& cs);
		virtual BOOL 	PreTranslateMessage(MSG *Msg);
		virtual void	Serialize(CArchive &ar);
		virtual	BOOL    SetControlStatus(UINT, bool, ControlBars);
		virtual void    SetStatusbarMsg(CString);
		virtual void    SetupToolBar();
		virtual void	UpdateFrame();
		virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

	  	  // protected data members
		AboutBox   m_AboutDialog;  // about dialog object
		CDoc 	   m_Doc;	   // the document object
		CMRU       m_MRU;          // the MRU list object
		CView	   m_View;	   // the view object

                UINT 	   m_win_x,        // deserialized window x position
                           m_win_y,        // deserialized window y position
                           m_win_width,    // deserialized window width
                           m_win_height;   // deserialized window height
		HWND       m_hadFocus;     // focus window on context help
		HCURSOR    m_hCursor;      // mainframe cursor
		CursorShape m_cursor_shape;// mainframe cursor arrow/help/wait
		WINDOWPLACEMENT m_Wndpl;   // window placement information

		static const size_t m_nMaxMRUEntries; // maximum frame MRU's 
		
	private:

};

/*==============================================================================

	Declare the global function that provides access to the
	CDoc class 							*/
	
CDoc& TheDoc();

/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

