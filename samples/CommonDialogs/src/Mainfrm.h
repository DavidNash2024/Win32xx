/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the CMainFrame class for the
	CommonDialogs SDI sample application using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein.
	
	This particular frame class contains features a fixed-size form for the
	display, with no resizing gripper tool at the end of the status bar,
	and provisions for selection of client background color, selection of
	edit box font, use of external serialization files, MRU lists, and
	context help for all controls on the frame and client area.

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

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/*******************************************************************************

	Declaration and Implementation of the NoResizeGripperStatusBar class

*=============================================================================*/
	class
NoResizeGripperStatusBar : public CStatusBar                            /*

	This class defines a status bar at the bottom of the frame having
	no resizing gripper box. To use this class, declare a member variable
	of the frame of this class, as, for example,
	
		NoResizeGripperStatusBar m_NoResizeGripperStatusBar;

	and then override the frame's GetStatusBar() method with the following:
	
		virtual CStatusBar& GetStatusBar() const
				{ return const_cast<NoResizeGripperStatusBar&>
				  (m_NoResizeGripperStatusBar); }

	The frame window will still resize, but no gripper id displayed. Remove
	the WS_THICKFRAME style option in the frame's PreCreate() member to
	make the frame be of fixed size.
*-----------------------------------------------------------------------------*/
{
	void PreCreate(CREATESTRUCT &cs)
	    { cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM; }
};

/*******************************************************************************

	Declaration of the CMainFrame class
	
********************************************************************************

	Local constants and types                               	*/

  // designation of where a control id appears, used when enabling/disabling
enum ControlBars {toolbar, mainmenu, both};

/*============================================================================*/
	class 
CMainFrame : public CFrame						/*

	This application's mainframe class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
	public:
		CMainFrame(void);
		virtual 	~CMainFrame(void){}
		
			void 	AddMRUEntry(LPCTSTR szMRUEntry)
				    { CFrame::AddMRUEntry (szMRUEntry);}
			BOOL	DoContextHelp(WPARAM wParam);
		virtual void    EmptyMRUList();
		virtual BOOL    EngageContextHelp();
		       AppHelp& GetAppHelp() {return m_AppHelp;}
			CDoc& 	GetDoc() { return m_Doc; }
			UINT    GetMRUSize() { return GetMRUEntries().size();}
		virtual CStatusBar& GetStatusBar() const
				{ return const_cast<NoResizeGripperStatusBar&>
				  (m_NoResizeGripperStatusBar); }
			CView&  GetView() {return m_View;}
		virtual void	RemoveMRUEntry(const CString& szMRUEntry)
				    {CFrame::RemoveMRUEntry(szMRUEntry);}
			void    SetSBBkColor(COLORREF clr)
				    { GetStatusBar().SendMessage(SB_SETBKCOLOR,
				      0, (LPARAM)clr);}
		virtual void 	SetWindowTitle(const CString& title);
		virtual void 	UpdateControlUIState();

		    COLORREF 	GetSBBkColor()
		    		{ return m_ColorChoice.GetTableColor(SBBg);}

	protected:
		virtual void    ConnectAppHelp();
		virtual void	GetCtlColors(UINT nCtlColor, UINT nID, UINT& fg,
				    UINT& bk, UINT& bg);
		 CRichEditView& GetREView() { return m_View.GetREView();}

		virtual void    LoadPersistentData();
			void    OnBogusMRU();
		virtual void    OnColorChoice();
		virtual BOOL 	OnCommand(WPARAM wParam, LPARAM lParam);
		virtual INT_PTR OnCtlColor(HDC, HWND, UINT);
		virtual int  	OnCreate(CREATESTRUCT& rcs);
		virtual void    OnEditFind();
		virtual void    OnEditReplace();
		virtual void    OnFileClose();
		virtual void    OnFileNew();
		virtual void    OnFileOpen();
		virtual void 	OnFileOpenMRU(UINT);
		virtual void    OnFilePageSetup();
		virtual void    OnFilePreview();
		virtual void    OnFilePrint();
		virtual void    OnFileSave();
		virtual void    OnFileSaveAs();
		virtual void    OnFontChoice();
		virtual void    InitCtlColors();
		virtual void    OnInitialUpdate(void);
		virtual void 	OnMenuUpdate(UINT nID);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual BOOL	OnProcessMRU(WPARAM wParam, LPARAM lParam);
		virtual BOOL    OnRichEditColor();
		virtual void 	OnTerminate(void);
		virtual void    OnWrapText();
		virtual void 	PreCreate(CREATESTRUCT& cs);
		virtual BOOL 	SaveRegistrySettings(void);
		virtual void	Serialize(CArchive &ar);
		virtual void    SetContextHelpMessages(void);
			BOOL	SetCheckStatus(UINT, BOOL, ControlBars);
			BOOL    SetEnableStatus(UINT, BOOL, ControlBars);
		virtual void 	SetReBarColors(COLORREF clrBkGnd1,
				    COLORREF clrBkGnd2, COLORREF clrBand1,
				    COLORREF clrBand2);
		virtual void    SetStatusbarMsg(CString);
		virtual BOOL 	SetThemeColors();
		virtual void    SetupToolBar(void);
		virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);
		virtual void 	UpdateMRUMenu();
		virtual void	ValidateMRU();

	private:
		  // private data members
		CDoc 	   m_Doc;	      // the document
		CView	   m_View;	      // the view
                UINT 	   m_win_x,           // serialized window x position
                           m_win_y,           // serialized window y position
                           m_width,           // serialized window width
                           m_height;          // serialized window height
		UINT	   m_nMaxMRU;	      // maximum MRU entries, this app
		HCURSOR    m_hCursor;         // current cursor shape
		CBitmap    m_colorbmp;        // for the color choice menuitem
		AppHelp    m_AppHelp;         // the context help object
		CBrush     m_br;              // backbround brush object
		BOOL       m_bWrapText;       // wrap text in rich edit if true.
		CColorChoice m_ColorChoice;   // the control color choice
		MyFontDialog m_FontChoice;    // edit control font
		WINDOWPLACEMENT m_Wndpl;      // window placement information
		MyFindReplaceDialog m_FindRepDialog;  // find-replace dialog
		NoResizeGripperStatusBar m_NoResizeGripperStatusBar;
};
/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

