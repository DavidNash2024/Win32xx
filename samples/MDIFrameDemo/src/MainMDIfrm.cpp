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

BOOL CMainMDIFrame::OnFileNew()
{
	// Creates the popup menu when the "New" toolbar button is pressed

	// Position the popup menu
	CToolBar& TB = GetToolBar();
	RECT rc = TB.GetItemRect(TB.CommandToIndex(IDM_FILE_NEW));
	TB.MapWindowPoints(NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_NEWMENU);
	CMenu PopupMenu = TopMenu.GetSubMenu(0);

	// Start the popup menu
	PopupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, &tpm);

	return TRUE;
}

BOOL CMainMDIFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:		 return OnFileNew();
	case IDM_FILE_NEWVIEW:	 return OnFileNewView();
	case IDM_FILE_NEWRECT:	 return OnFileNewRect();
	case IDM_FILE_NEWTEXT:	 return OnFileNewText();
	case IDM_FILE_NEWMAX:	 return OnFileNewMax();
	case IDM_FILE_NEWTREE:	 return OnFileNewTree();
	case IDM_FILE_NEWLIST:	 return OnFileNewList();
	case IDM_FILE_CLOSE:	 return OnFileClose();
	case IDM_FILE_EXIT:		 return OnFileExit();
	case IDM_HELP_ABOUT:	 return OnHelp();
	case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:	 return OnViewToolBar();
	case IDW_MDI_ARRANGE:	 return OnMDIIconArrange();
	case IDW_MDI_CASCADE:	 return OnMDICascade();
	case IDW_MDI_CLOSEALL:	 return OnMDICloseAll();
	case IDW_MDI_TILE:		 return OnMDITile();
	default:    // Pass to active child...
		{
			if (GetActiveMDIChild())
				GetActiveMDIChild()->SendMessage(WM_COMMAND, wParam, lParam);
		}
		break ;

	}

	return FALSE;
}

int CMainMDIFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CMDIFrame::OnCreate(pcs);
}

BOOL CMainMDIFrame::OnFileClose()
{
	// Ask the active MDI window to close
	GetActiveMDIChild()->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);

	return TRUE;
}

BOOL CMainMDIFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);

	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewList()
{
	AddMDIChild(new CMDIChildListView);	// This pointer is stored in a Shared_Ptr
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewMax()
{
	AddMDIChild(new CMDIChildMax);	// This pointer is stored in a Shared_Ptr
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewRect()
{
	AddMDIChild(new CMDIChildRect);	// This pointer is stored in a Shared_Ptr
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewText()
{
	AddMDIChild(new CMDIChildText);	// This pointer is stored in a Shared_Ptr
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewTree()
{
	AddMDIChild(new CMDIChildTreeView);	// This pointer is stored in a Shared_Ptr
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewView()
{
	AddMDIChild(new CMDIChildSimple);	// This pointer is stored in a Shared_Ptr
	return TRUE;
}

BOOL CMainMDIFrame::OnMDICascade()
{ 
	MDICascade(); 
	return TRUE; 
}

BOOL CMainMDIFrame::OnMDICloseAll()
{ 
	RemoveAllMDIChildren(); 
	return TRUE;
}

BOOL CMainMDIFrame::OnMDIIconArrange() 
{
	MDIIconArrange();
	return TRUE; 
}

BOOL CMainMDIFrame::OnMDITile()
{ 
	MDITile(); 
	return TRUE; 
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
			if (((LPNMHDR)lParam)->hwndFrom == GetToolBar())
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
	AddMenuIcon(IDM_FILE_NEWVIEW, GetApp().LoadIcon(IDI_VIEW));
	AddMenuIcon(IDM_FILE_NEWRECT, GetApp().LoadIcon(IDI_RECT));
	AddMenuIcon(IDM_FILE_NEWMAX, GetApp().LoadIcon(IDI_MAX));
	AddMenuIcon(IDM_FILE_NEWTEXT, GetApp().LoadIcon(IDI_TEXT));
	AddMenuIcon(IDM_FILE_NEWLIST, GetApp().LoadIcon(IDI_FILES));
	AddMenuIcon(IDM_FILE_NEWTREE, GetApp().LoadIcon(IDI_CLASSES));

	// Configure the "New" toolbar button to bring up a menu
	// Setting this style requires comctl32.dll version 4.72 or later
	if (GetComCtlVersion() >= 472)
	{
		GetToolBar().SetButtonStyle(IDM_FILE_NEW, BTNS_WHOLEDROPDOWN);
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

