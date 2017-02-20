////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class

#include "stdafx.h"
#include "mainMDIfrm.h"
#include "SimpleMDIChild.h"
#include "Files.h"
#include "resource.h"


CMainMDIFrame::CMainMDIFrame()
{
	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\DockMDI Frame"));
}

CMainMDIFrame::~CMainMDIFrame()
{
}

BOOL CMainMDIFrame::OnFileOpen()
{
	CFileDialog FileDlg(TRUE);

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// TODO:
		// Add your own code here. Refer to the tutorial for additional information
	}

	return TRUE;
}

BOOL CMainMDIFrame::OnFileSave()
{
	CFileDialog FileDlg(FALSE);

	// Bring up the file save dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// TODO:
		// Add your own code here. Refer to the tutorial for additional information
	}

	return TRUE;
}

BOOL CMainMDIFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = GetHwnd();

	// Retrieve the printer DC
	PrintDlg( &pd );

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

void CMainMDIFrame::OnInitialUpdate()
{
	TRACE("MDI Frame started \n");
	//The frame is now created.
	//Place any additional startup code here.

	// Add some Dockers to the MDI Frame
	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each docker
	int DockWidth = 150;
	CDocker* pDock1 = AddDockedChild(new CDockFiles, DS_DOCKED_LEFT | dwStyle, DockWidth);
	CDocker* pDock2 = AddDockedChild(new CDockFiles, DS_DOCKED_RIGHT | dwStyle, DockWidth);

	assert (pDock1->GetContainer());
	assert (pDock2->GetContainer());
	pDock1->GetContainer()->SetHideSingleTab(TRUE);
	pDock2->GetContainer()->SetHideSingleTab(TRUE);

	// Add some  MDI children
	AddMDIChild(new CSimpleMDIChild);
	AddMDIChild(new CSimpleMDIChild);
}

BOOL CMainMDIFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	switch (nID)
	{
	case IDM_FILE_NEWDOCK:		return OnFileNewDocker();
	case IDM_FILE_NEWMDI:		return OnFileNewMDI();
	case IDM_FILE_CLOSE:		return OnFileClose();
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSave();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDW_MDI_ARRANGE:		return OnMDIIconArrange();
	case IDW_MDI_CASCADE:		return OnMDICascade();
	case IDW_MDI_CLOSEALL:		return OnMDICloseAll();
	case IDW_MDI_TILE:			return OnMDITile();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDM_HELP_ABOUT:		return OnHelp();
	default:
		{
			// Pass to active child...
			if (GetActiveMDIChild())
				GetActiveMDIChild()->SendMessage(WM_COMMAND, wParam, lParam);
		}
		break ;
	}
	return FALSE;
}

int CMainMDIFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar


	// call the base class function
	return CMDIDockFrame::OnCreate(cs);
}

BOOL CMainMDIFrame::OnFileClose()
{
	GetActiveMDIChild()->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	return TRUE;
}

BOOL CMainMDIFrame::OnFileExit()
{
	// Issue a close request to the frame
	CMDIDockFrame::PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewMDI()
{
	// Create a new MDI child
	AddMDIChild(new CSimpleMDIChild);
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNewDocker()
{
	AddDockedChild(new CDockFiles, DS_DOCKED_LEFT | DS_CLIENTEDGE, 150);
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

void CMainMDIFrame::SetupMenuIcons()
{
	// Add the default set of menu icons from the toolbar.
	CMDIDockFrame::SetupMenuIcons();

	// Add an extra icon for the New Docker menu item
	AddMenuIcon(IDM_FILE_NEWDOCK, GetApp().LoadIcon(IDW_MAIN));
}

void CMainMDIFrame::SetupToolBar()
{
	// Define the resource IDs for the toolbar
	AddToolBarButton( IDM_FILE_NEWMDI);
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT   );
	AddToolBarButton( IDM_EDIT_COPY  );
	AddToolBarButton( IDM_EDIT_PASTE );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainMDIFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

//	pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

