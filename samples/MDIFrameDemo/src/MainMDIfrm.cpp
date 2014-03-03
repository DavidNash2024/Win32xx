////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class

#include "stdafx.h"
#include "resource.h"
#include "mainMDIfrm.h"
#include "MDIChildSimple.h"
#include "MDIChildRect.h"
#include "MDIChildMax.h"
#include "MDIChildText.h"
#include "MDIChildTreeView.h"
#include "MDIChildListView.h"


CMainMDIFrame::CMainMDIFrame()
{
	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\MDIFrameDemo"));
}

CMainMDIFrame::~CMainMDIFrame()
{
}

void CMainMDIFrame::OnInitialUpdate()
{
	//The frame is now created.
	//Place any additional startup code here.
}

void CMainMDIFrame::OnFileNew()
{
	// Creates the popup menu when the "New" toolbar button is pressed

	// Position the popup menu
	CToolBar* pTB = GetToolBar();
	RECT rc = pTB->GetItemRect(pTB->CommandToIndex(IDM_FILE_NEW));
	pTB->MapWindowPoints(NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_NEWMENU);
	CMenu* pPopupMenu = TopMenu.GetSubMenu(0);

	// Start the popup menu
	pPopupMenu->TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, this, &tpm);
}

BOOL CMainMDIFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:		 OnFileNew();			 return TRUE;
	case IDM_FILE_NEWVIEW:	 OnFileNewView();		 return TRUE;
	case IDM_FILE_NEWRECT:	 OnFileNewRect();		 return TRUE;
	case IDM_FILE_NEWTEXT:	 OnFileNewText();		 return TRUE;
	case IDM_FILE_NEWMAX:	 OnFileNewMax();		 return TRUE;
	case IDM_FILE_NEWTREE:	 OnFileNewTree();		 return TRUE;
	case IDM_FILE_NEWLIST:	 OnFileNewList();		 return TRUE;
	case IDM_FILE_CLOSE:	 OnFileClose();			 return TRUE;
	case IDM_FILE_EXIT:		 OnFileExit();			 return TRUE;
	case IDM_HELP_ABOUT:	 OnHelp();				 return TRUE;
	case IDW_VIEW_STATUSBAR: OnViewStatusBar();		 return TRUE;
	case IDW_VIEW_TOOLBAR:	 OnViewToolBar();		 return TRUE;
	case IDW_MDI_ARRANGE:	 MDIIconArrange();		 return TRUE;
	case IDW_MDI_CASCADE:	 MDICascade();			 return TRUE;
	case IDW_MDI_CLOSEALL:	 RemoveAllMDIChildren(); return TRUE;
	case IDW_MDI_TILE:		 MDITile();				 return TRUE;
	default:    // Pass to active child...
		{
			if (GetActiveMDIChild())
				GetActiveMDIChild()->SendMessage(WM_COMMAND, wParam, lParam);
		}
		break ;

	}

	return FALSE;
}

void CMainMDIFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseReBar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a toolbar

	// call the base class function
	CMDIFrame::OnCreate();
}

void CMainMDIFrame::OnFileClose()
{
	// Ask the active MDI window to close
	GetActiveMDIChild()->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CMainMDIFrame::OnFileExit()
{
	// The application ends when the frame is destroyed
	Destroy();
}

void CMainMDIFrame::OnFileNewList()
{
	AddMDIChild(new CMDIChildListView);	// CMDIFrame::RemoveMDIChild deletes this pointer
}

void CMainMDIFrame::OnFileNewMax()
{
	AddMDIChild(new CMDIChildMax);	// This pointer is stored in a Shared_Ptr
}

void CMainMDIFrame::OnFileNewRect()
{
	AddMDIChild(new CMDIChildRect);	// This pointer is stored in a Shared_Ptr
}

void CMainMDIFrame::OnFileNewText()
{
	AddMDIChild(new CMDIChildText);	// This pointer is stored in a Shared_Ptr
}

void CMainMDIFrame::OnFileNewTree()
{
	AddMDIChild(new CMDIChildTreeView);	// This pointer is stored in a Shared_Ptr
}

void CMainMDIFrame::OnFileNewView()
{
	AddMDIChild(new CMDIChildSimple);	// This pointer is stored in a Shared_Ptr
}

LRESULT CMainMDIFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Notification from our dropdown button is recieved if Comctl32.dll version
	// is 4.70 or later (IE v3 required).
    switch(((LPNMHDR)lParam)->code)
	{
 		//Menu for dropdown toolbar button
		case TBN_DROPDOWN:
		{
			if (((LPNMHDR)lParam)->hwndFrom == GetToolBar()->GetHwnd())
				OnFileNew();
		}
		break;

	} //switch LPNMHDR

	return CMDIFrame::OnNotify(wParam, lParam);
}

void CMainMDIFrame::SetupToolBar()
{
	// Define the resource IDs for the toolbar
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN,  FALSE );
	AddToolBarButton( IDM_FILE_SAVE,  FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT );

	// Use larger buttons with seperate imagelists for normal, hot and disabled buttons.
	SetToolBarImages(RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_FILE_NEWVIEW, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_VIEW)));
	AddMenuIcon(IDM_FILE_NEWRECT, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_RECT)));
	AddMenuIcon(IDM_FILE_NEWMAX,  ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_MAX)));
	AddMenuIcon(IDM_FILE_NEWTEXT, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_TEXT)));
	AddMenuIcon(IDM_FILE_NEWLIST, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_FILES)));
	AddMenuIcon(IDM_FILE_NEWTREE, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_CLASSES)));

	// Configure the "New" toolbar button to bring up a menu
	// Setting this style requires comctl32.dll version 4.72 or later
	if (GetComCtlVersion() >= 472)
	{
		GetToolBar()->SetButtonStyle(IDM_FILE_NEW, BTNS_WHOLEDROPDOWN);
	}
}

LRESULT CMainMDIFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		 Add case statements for each messages to be handled here
//	}

//	pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

