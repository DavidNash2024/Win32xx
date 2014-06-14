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
	LoadRegistrySettings(_T("Win32++\\MDI Frame"));

	GetDockClient()->SetDock(this);
	SetView(*GetDockClient());
}

CMainMDIFrame::~CMainMDIFrame()
{
}

void CMainMDIFrame::OnFileOpen()
{
	// Bring up the file open dialog
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

void CMainMDIFrame::OnFileSave()
{
	// Bring up the file save dialog.
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

void CMainMDIFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd = {0};
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = CWnd::m_hWnd;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}


void CMainMDIFrame::OnInitialUpdate()
{
	TRACE("MDI Frame started \n");
	//The frame is now created.
	//Place any additional startup code here.

	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each docker
	CDocker* pDockRight  = AddDockedChild(new CDockFiles, DS_DOCKED_LEFT | dwStyle, 250, ID_DOCK_FILES);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_LEFT | dwStyle, 250, ID_DOCK_FILES);
}

BOOL CMainMDIFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:		AddMDIChild(new CSimpleMDIChild); return TRUE;
	case IDM_FILE_CLOSE:		OnFileClose();				return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();				return TRUE;
	case IDM_FILE_SAVE:			OnFileSave();				return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSave();				return TRUE;
	case IDM_FILE_PRINT:		OnFilePrint();				return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();			return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();			return TRUE;
	case IDW_MDI_ARRANGE:		MDIIconArrange();			return TRUE;
	case IDW_MDI_CASCADE:		MDICascade();				return TRUE;
	case IDW_MDI_CLOSEALL:		RemoveAllMDIChildren();		return TRUE;
	case IDW_MDI_TILE:			MDITile();					return TRUE;
	case IDM_FILE_EXIT:			OnFileExit();				return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();					return TRUE;
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

int CMainMDIFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CDockFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bUseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_bUseMenuStatus = FALSE;		// Don't show menu descriptions in the StatusBar
	// m_bUseReBar = FALSE;				// Don't use a ReBar
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a ToolBar


	// call the base class function
	return CMDIFrame::OnCreate(pcs);
}

void CMainMDIFrame::OnFileClose()
{
	GetActiveMDIChild()->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CMainMDIFrame::OnFileExit()
{
	// Issue a close request to the frame
	CMDIFrame::PostMessage(WM_CLOSE);
}

void CMainMDIFrame::RecalcLayout()
{
	CWnd* pView = GetView();
	if ((!pView) || (!pView->GetHwnd()))
		return;

	// Resize the status bar
	if (GetStatusBar()->IsWindow() && m_bShowStatusBar)
	{
		GetStatusBar()->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW);
		GetStatusBar()->Invalidate();
		GetStatusBar()->SetWindowText(GetStatusText());
		SetStatusIndicators();
	}

	// Resize the rebar or toolbar
	if (IsReBarUsed())
	{
		GetReBar()->SendMessage(WM_SIZE, 0L, 0L);
		GetReBar()->Invalidate();
	}
	else if (m_bUseToolBar && m_bShowToolBar && GetToolBar()->IsWindow())
		GetToolBar()->SendMessage(TB_AUTOSIZE, 0L, 0L);

	// Position the view window
//	pView->SetWindowPos( NULL, GetViewRect(), SWP_SHOWWINDOW);
	RecalcDockLayout();

	// Adjust rebar bands
	if (IsReBarUsed())
	{
		if (GetReBarTheme()->UseThemes && GetReBarTheme()->BandsLeft)
			GetReBar()->MoveBandsLeft();

		if (IsMenuBarUsed())
			SetMenuBarBandSize();
	}
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
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

//	pass unhandled messages on for default processing
	return CMDIFrame::WndProcDefault(uMsg, wParam, lParam);
}

HWND CDockMDIClient::Create(CWnd* pParent)
{
	assert(pParent != 0);

	CLIENTCREATESTRUCT clientcreate ;
	clientcreate.hWindowMenu  = 0;
	clientcreate.idFirstChild = IDW_FIRSTCHILD ;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | MDIS_ALLCHILDSTYLES;
	HWND hWndParent = pParent? pParent->GetHwnd() : 0;

	// Create the view window
	CreateEx(WS_EX_CLIENTEDGE, _T("MDICLient"), TEXT(""), dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate);

	return m_hWnd;
}

LRESULT CDockMDIClient::OnMDIActivate(WPARAM wParam, LPARAM lParam)
{
	// Suppress redraw to avoid flicker when activating maximised MDI children
	SetRedraw(FALSE);
	LRESULT lr = CallWindowProc(GetPrevWindowProc(), WM_MDIACTIVATE, wParam, lParam);
	SetRedraw(TRUE);
	RedrawWindow(0, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

	return lr;
}

LRESULT CDockMDIClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	// Do default processing first
	CallWindowProc(GetPrevWindowProc(), WM_MDIDESTROY, wParam, lParam);

	// Now remove MDI child
	GetMDIFrame()->RemoveMDIChild((HWND) wParam);

	return 0L;
}

LRESULT CDockMDIClient::OnMDISetMenu(WPARAM wParam, LPARAM lParam)
{
	if (GetMDIFrame()->IsMenuBarUsed())
	{
		return 0L;
	}

	return FinalWindowProc(WM_MDISETMENU, wParam, lParam);
}

LRESULT CDockMDIClient::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MDIACTIVATE: return OnMDIActivate(wParam, lParam);
	case WM_MDIDESTROY:	return OnMDIDestroy(wParam, lParam);
	case WM_MDISETMENU:	return OnMDISetMenu(wParam, lParam);
	}
	return CWnd::WndProcDefault(uMsg, wParam, lParam);
}