////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "FormApp.h"
#include "mainfrm.h"


// A global function to provide access to the CFormDoc class
CFormDoc& GetDoc()
{
	return GetSdiApp().GetMainFrame().GetDoc();
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

void CMainFrame::LoadRegistrySettings(LPCTSTR szKeyName)
{
	CFrame::LoadRegistrySettings(szKeyName);
	GetDoc().LoadDocRegistry(GetRegistryKeyName().c_str());
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
	case ID_CHECK_A:
		m_SdiView.OnCheckA();
		return TRUE;
	case ID_CHECK_B:
		m_SdiView.OnCheckB();
		return TRUE;
	case ID_CHECK_C:
		m_SdiView.OnCheckC();
		return TRUE;

	case ID_RADIO_A:
	case ID_RADIO_B:
	case ID_RADIO_C:
		m_SdiView.OnRangeOfIds_Radio(wpLo - ID_RADIO_A);
		return TRUE;

	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;

    case IDM_HELP_ABOUT:
      // Display the help dialog
      OnHelp();
      return TRUE;
    }

  return FALSE;
} 

void CMainFrame::OnCreate()
{
	// Override OnCreate to customise the frame creation

	// Use the default frame creation
	// m_bUseThemes  = FALSE;
	// m_bUseRebar   = FALSE;
	// m_bUseToolbar = FALSE;
	CFrame::OnCreate();

}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.
	
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	CFrame::PreCreate(cs);
//	cs.cx = 500;
//	cs.cy = 420;
}

void CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();
	GetDoc().SaveDocRegistry(GetRegistryKeyName().c_str());
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );        // Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( 0 );        // Separator
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );        // Separator
	AddToolbarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
/*	switch (uMsg)
	{
	// Suppress resizing
	case WM_SIZING:
		{
			LPRECT pRect = (LPRECT)lParam;
			pRect->right = pRect->left + 500;
			pRect->bottom = pRect->top + 420;
		}
		return TRUE; 
	} */

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

void CMainFrame::OnUpdateCheckA(UINT nID)
{
	BOOL bCheck = GetDoc().GetCheckA();
	::CheckMenuItem(GetFrameMenu(), nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckB(UINT nID)
{
	BOOL bCheck = GetDoc().GetCheckB();
	CheckMenuItem(GetFrameMenu(), nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckC(UINT nID)
{
	BOOL bCheck = GetDoc().GetCheckC();
	CheckMenuItem(GetFrameMenu(), nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateRangeOfIds_Radio(UINT nID)
{
	UINT adjId = nID - ID_RADIO_A;
	UINT curRadio = GetDoc().GetRadio();
	BOOL bCheck = (curRadio == adjId);
	int nFileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
	HMENU hRadio = ::GetSubMenu(GetFrameMenu(), nFileItem);
	if (bCheck)  
		::CheckMenuRadioItem(hRadio, ID_RADIO_A, ID_RADIO_C, nID, 0);
}
