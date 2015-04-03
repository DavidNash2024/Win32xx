////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "FormApp.h"
#include "mainfrm.h"


// A global function to provide access to the CFormDoc class
CFormDoc* GetDoc()
{
	return GetSdiApp()->GetMainFrame()->GetDoc();
}


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_SdiView(IDD_DIALOG1)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_SdiView as the view window of the frame
	SetView(m_SdiView);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\SdiDocViewForm"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR szKeyName)
{
	CFrame::LoadRegistrySettings(szKeyName);
	GetDoc()->LoadDocRegistry(GetRegistryKeyName().c_str());

	return TRUE;
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
	// Update the check state of the various menu items
	switch (nID) 
	{
	case ID_CHECK_A:
		OnUpdateCheckA(nID);
		break;
	case ID_CHECK_B:
		OnUpdateCheckB(nID);
		break;
	case ID_CHECK_C:
		OnUpdateCheckC(nID);
		break;
	}

	if ((nID >= ID_RADIO_A) && (nID <= ID_RADIO_C)) 
		OnUpdateRangeOfIds_Radio(nID);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	WORD wpLo = LOWORD(wParam);
    
	switch(LOWORD(wParam))
	{
	case ID_CHECK_A:		 m_SdiView.OnCheckA();	return TRUE;
	case ID_CHECK_B:		 m_SdiView.OnCheckB();	return TRUE;
	case ID_CHECK_C:		 m_SdiView.OnCheckC();	return TRUE;
	case IDM_FILE_EXIT:		 OnFileExit();			return TRUE;
	case IDW_VIEW_STATUSBAR: OnViewStatusBar();		return TRUE;
	case IDW_VIEW_TOOLBAR:	 OnViewToolBar();		return TRUE;
    case IDM_HELP_ABOUT:	 OnHelp();				return TRUE;
	case ID_RADIO_A:
	case ID_RADIO_B:		// intentionally blank
	case ID_RADIO_C:		m_SdiView.OnRangeOfIds_Radio(wpLo - ID_RADIO_A);	return TRUE;
    }

  return FALSE;
} 

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
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
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.
	
}

void CMainFrame::OnUpdateCheckA(UINT nID)
{
	BOOL bCheck = GetDoc()->GetCheckA();
	GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckB(UINT nID)
{
	BOOL bCheck = GetDoc()->GetCheckB();
	GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckC(UINT nID)
{
	BOOL bCheck = GetDoc()->GetCheckC();
	GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateRangeOfIds_Radio(UINT nID)
{
	UINT adjId = nID - ID_RADIO_A;
	UINT curRadio = GetDoc()->GetRadio();
	BOOL bCheck = (curRadio == adjId);
	int nFileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
	CMenu RadioMenu = GetFrameMenu().GetSubMenu(nFileItem);
	if (bCheck)  
		RadioMenu.CheckMenuRadioItem(ID_RADIO_A, ID_RADIO_C, nID, 0);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	CFrame::PreCreate(cs);
//	cs.cx = 500;
//	cs.cy = 420;
}

BOOL CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();
	GetDoc()->SaveDocRegistry(GetRegistryKeyName().c_str());

	return TRUE;
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );        // Separator
	AddToolBarButton( IDM_EDIT_CUT   );
	AddToolBarButton( IDM_EDIT_COPY  );
	AddToolBarButton( IDM_EDIT_PASTE );
	AddToolBarButton( 0 );        // Separator
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );        // Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	} 

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

