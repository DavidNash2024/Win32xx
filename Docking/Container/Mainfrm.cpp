////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "ContainerApp.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_DockView as the view window of the frame
	SetView(m_DockView);

	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Container Docking"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::AddDocked(DockInfo di, CDockable* pDock)
{
	switch(di.DockID)
	{
	case ID_CONTAINCLASSES1:
		pDock->AddDockedChild(new CDockClasses, di.DockStyle, di.DockWidth, ID_CONTAINCLASSES1);
		break;
	case ID_CONTAINCLASSES2:
		pDock->AddDockedChild(new CDockClasses, di.DockStyle, di.DockWidth, ID_CONTAINCLASSES2);					
		break;
	case ID_CONTAINFILES1:
		pDock->AddDockedChild(new CDockFiles, di.DockStyle, di.DockWidth, ID_CONTAINFILES1);
		break;
	case ID_CONTAINFILES2:
		pDock->AddDockedChild(new CDockFiles, di.DockStyle, di.DockWidth, ID_CONTAINFILES2);
		break;
	case ID_OUTPUT1:
		pDock->AddDockedChild(new CDockOutput, di.DockStyle, di.DockWidth, ID_OUTPUT1);
		break;
	case ID_OUTPUT2:
		pDock->AddDockedChild(new CDockOutput, di.DockStyle, di.DockWidth, ID_OUTPUT2);
		break;
	case ID_TEXT1:
		pDock->AddDockedChild(new CDockText, di.DockStyle, di.DockWidth, ID_TEXT1);
		break;
	case ID_TEXT2:
		pDock->AddDockedChild(new CDockText, di.DockStyle, di.DockWidth, ID_TEXT2);
		break;
	default:
		TRACE(_T("Unknown Dock ID\n"));
		break;
	}
}

void CMainFrame::AddUndocked(DockInfo di)
{
	switch(di.DockID)
	{
	case ID_CONTAINCLASSES1:
		m_DockView.AddUndockedChild(new CDockClasses, di.DockStyle, di.DockWidth, di.Rect, ID_CONTAINCLASSES1);
		break;
	case ID_CONTAINCLASSES2:
		m_DockView.AddUndockedChild(new CDockClasses, di.DockStyle, di.DockWidth, di.Rect, ID_CONTAINCLASSES2);					
		break;
	case ID_CONTAINFILES1:
		m_DockView.AddUndockedChild(new CDockFiles, di.DockStyle, di.DockWidth, di.Rect, ID_CONTAINFILES1);
		break;
	case ID_CONTAINFILES2:
		m_DockView.AddUndockedChild(new CDockFiles, di.DockStyle, di.DockWidth, di.Rect, ID_CONTAINFILES2);
		break;
	case ID_OUTPUT1:
		m_DockView.AddUndockedChild(new CDockOutput, di.DockStyle, di.DockWidth, di.Rect, ID_OUTPUT1);
		break;
	case ID_OUTPUT2:
		m_DockView.AddUndockedChild(new CDockOutput, di.DockStyle, di.DockWidth, di.Rect, ID_OUTPUT2);
		break;
	case ID_TEXT1:
		m_DockView.AddUndockedChild(new CDockText, di.DockStyle, di.DockWidth, di.Rect, ID_TEXT1);
		break;
	case ID_TEXT2:
		m_DockView.AddUndockedChild(new CDockText, di.DockStyle, di.DockWidth, di.Rect, ID_TEXT2);
	default:
		TRACE(_T("Unknown Dock ID\n"));
		break; 
	}
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// OnCommand responds to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_DOCK_DEFAULT:
		m_DockView.CloseAllDockables();
		LoadDefaultDockables();
		return TRUE;
	case IDM_DOCK_CLOSEALL:
		m_DockView.CloseAllDockables();
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
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes

	// call the base class function
	CFrame::OnCreate();
		
}

void CMainFrame::OnInitialUpdate()
{
	m_DockView.SetDockStyle(DS_CLIENTEDGE);

	if (0 == GetRegistryKeyName().size())
		LoadDefaultDockables();
	else
		LoadRegistryDockables();

	// Ensure we have some docked/undocked windows
	if (0 == m_DockView.GetAllDockables().size())
		LoadDefaultDockables();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::LoadDefaultDockables()
{
	// Note: The  DockIDs are used for saving/restoring the dockables state in the registry

	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each dockable
	
	// Add the remaining dockables
	CDockable* pDockRight  = m_DockView.AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | dwStyle, 200, ID_CONTAINCLASSES1);	
	CDockable* pDockBottom = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_TEXT1);

	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINFILES1);
	pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINCLASSES2);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINFILES2);

	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_OUTPUT1);
	pDockBottom->AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | dwStyle, 100, ID_TEXT2);
	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_OUTPUT2);
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);
	
	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

void CMainFrame::LoadRegistryDockables()
{
	if (0 != GetRegistryKeyName().size())
	{
		std::vector<DockInfo> vDockList;
		tString tsKey = _T("Software\\") + GetRegistryKeyName() + _T("\\Dock Windows");
		HKEY hKey = 0;
		RegOpenKeyEx(HKEY_CURRENT_USER, tsKey.c_str(), 0, KEY_READ, &hKey);
		if (hKey)
		{
			DWORD dwType = REG_BINARY;
			DWORD BufferSize = sizeof(DockInfo);
			DockInfo di;
			int i = 0;
			TCHAR szNumber[16];
			tString tsSubKey = _T("DockChild");
			tsSubKey += _itot(i, szNumber, 10);
			
			// Fill the DockList vector from the registry
			while (0 == RegQueryValueEx(hKey, tsSubKey.c_str(), NULL, &dwType, (LPBYTE)&di, &BufferSize))
			{
				vDockList.push_back(di);
				i++;
				tsSubKey = _T("DockChild");
				tsSubKey += _itot(i, szNumber, 10);
			}
			
			RegCloseKey(hKey);
		}

		// Add dockables without parents first
		for (int i = vDockList.size() -1; i >= 0; --i)
		{
			DockInfo di = vDockList[i];
			if (di.DockParentID == 0)
			{
				if (di.DockStyle & 0xF)
					AddDocked(di, &m_DockView);
				else
					AddUndocked(di);

				vDockList.erase(vDockList.begin() + i);
			}
		}

		// Add remaining dockables
		while (vDockList.size() > 0)
		{
			bool bFound = false;
			std::vector<DockInfo>::iterator iter;
			for (iter = vDockList.begin(); iter < vDockList.end(); ++iter)
			{
				CDockable* pDock = m_DockView.GetDockFromID((*iter).DockParentID);
				if (pDock != 0)
				{
					AddDocked(*iter, pDock);
					bFound = true;
					vDockList.erase(iter);
					break;
				}
			}

			if (!bFound)
			{
				TRACE(_T("Orphaned dockables !!! \n"));
				break;
			}
		}	
	}	
}

void CMainFrame::SaveDockables()
{
	m_DockView.CheckDockables();
	// NOTE: This function assumes that each dockable has a unique DockID

	std::vector<CDockable*>::iterator iter;
	std::vector<DockInfo> vDockList;

	if (0 != GetRegistryKeyName().size())
	{
		// Fill the DockList vector with the docking information
		for (iter = m_DockView.GetAllDockables().begin(); iter <  m_DockView.GetAllDockables().end(); ++iter)
		{
			DockInfo di	 = {0};
			di.DockID	 = (*iter)->GetDockID();
			di.DockStyle = (*iter)->GetDockStyle();
			di.DockWidth = (*iter)->GetDockWidth();
			di.Rect		 = (*iter)->GetWindowRect();
			if ((*iter)->GetDockParent())
				di.DockParentID = (*iter)->GetDockParent()->GetDockID();
			
			vDockList.push_back(di);
		}

		tString tsKeyName = _T("Software\\") + GetRegistryKeyName();
		HKEY hKey = NULL;
		HKEY hKeyDock = NULL;
		if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		RegDeleteKey(hKey, _T("Dock Windows"));
		if (RegCreateKeyEx(hKey, _T("Dock Windows"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyDock, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		// Add the Dock windows information to the registry
		for (size_t u = 0; u < vDockList.size(); ++u)
		{
			DockInfo di = vDockList[u];
			TCHAR szNumber[16];
			tString tsSubKey = _T("DockChild");
			tsSubKey += _itot(u, szNumber, 10);
			RegSetValueEx(hKeyDock, tsSubKey.c_str(), 0, REG_BINARY, (LPBYTE)&di, sizeof(DockInfo));
		}

		RegCloseKey(hKeyDock);
		RegCloseKey(hKey);
	}
}

void CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();
	SaveDockables();
}

