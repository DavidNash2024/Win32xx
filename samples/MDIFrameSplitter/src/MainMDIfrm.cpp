////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class

#include "stdafx.h"
#include "mainMDIfrm.h"
#include "SimpleMDIChild.h"
#include "resource.h"


CMainMDIFrame::CMainMDIFrame()
{
	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\MDIFrameSplitter"));
}

CMainMDIFrame::~CMainMDIFrame()
{
}

BOOL CMainMDIFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:			return OnFileNew();
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
	default:    // Pass to active child...
		{
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
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CMDIFrame::OnCreate(cs);
}

BOOL CMainMDIFrame::OnFileOpen()
{
	// Bring up the file open dialog
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information
	return TRUE;
}

BOOL CMainMDIFrame::OnFileSave()
{
	// Bring up the file save dialog.
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
	return TRUE;
}

BOOL CMainMDIFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = *this;

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
}

BOOL CMainMDIFrame::OnFileClose()
{
	// Close the active MDI window
	GetActiveMDIChild()->SendMessage(WM_CLOSE, 0, 0);
	return TRUE;
}

BOOL CMainMDIFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainMDIFrame::OnFileNew()
{
	AddMDIChild(new CSimpleMDIChild);
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

void CMainMDIFrame::SetupToolBar()
{
	// Define the resource IDs for the toolbar
	AddToolBarButton( IDM_FILE_NEW   );
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
	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:
		// Called when a MDI child is activated with a mouse click
 		{
			// Redraw all MDI children to update docker caption
			std::vector<MDIChildPtr>::const_iterator iter;
			for (iter = GetAllMDIChildren().begin(); iter < GetAllMDIChildren().end(); ++iter)
			{
				(*iter)->RedrawWindow();
			}
		}
		break;
	}

//	pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

