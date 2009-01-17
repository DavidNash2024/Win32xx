////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
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
	LoadRegistrySettings(_T("Win32++\\Simple Docking"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
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
}

void CMainFrame::LoadDefaultDockables()
{
	// Note: The  DockIDs are used for saving/restoring the dockables state in the registry

	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each dockable

	CDockable* pDockLeft   = m_DockView.AddDockedChild(new CDockClass, DS_DOCKED_LEFT | dwStyle, 200, ID_CLASS1);
	CDockable* pDockRight  = m_DockView.AddDockedChild(new CDockClass, DS_DOCKED_RIGHT | dwStyle, 200, ID_CLASS2);
	CDockable* pDockTop    = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_TOP | dwStyle, 100, ID_TEXT1);
	CDockable* pDockBottom = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_TEXT2);

	pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM | dwStyle, 150, ID_FILES1);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM | dwStyle, 150, ID_FILES2);
	pDockTop->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT | dwStyle, 100, ID_SIMPLE1);
	pDockBottom->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT | dwStyle, 100, ID_SIMPLE2);
}

void CMainFrame::LoadRegistryDockables()
{
	if (0 != GetRegistryKeyName().size())
	{
		tString tsKey = _T("Software\\") + GetRegistryKeyName() + _T("\\Docked Windows");
		HKEY hKey = 0;
		RegOpenKeyEx(HKEY_CURRENT_USER, tsKey.c_str(), 0, KEY_READ, &hKey);
		if (hKey)
		{
			DWORD dwType = REG_BINARY;
			DWORD BufferSize = sizeof(DockedInfo);
			DockedInfo di;

			int i = 0;
			TCHAR szNumber[16];
			tString tsSubKey = _T("DockChild");
			tsSubKey += _itot(i, szNumber, 10);

			while (0 == RegQueryValueEx(hKey, tsSubKey.c_str(), NULL, &dwType, (LPBYTE)&di, &BufferSize))
			{
				CDockable* pDock = &m_DockView;

				if (-1 == di.DockParentID)
				{
					switch(di.DockID)
					{
					case ID_CLASS1:
						pDock->AddUndockedChild(new CDockClass, di.DockStyle, di.DockWidth, di.Rect, ID_CLASS1);
						break;
					case ID_CLASS2:
						pDock->AddUndockedChild(new CDockClass, di.DockStyle, di.DockWidth, di.Rect, ID_CLASS2);					
						break;
					case ID_FILES1:
						pDock->AddUndockedChild(new CDockFiles, di.DockStyle, di.DockWidth, di.Rect, ID_FILES1);
						break;
					case ID_FILES2:
						pDock->AddUndockedChild(new CDockFiles, di.DockStyle, di.DockWidth, di.Rect, ID_FILES2);
						break;
					case ID_SIMPLE1:
						pDock->AddUndockedChild(new CDockSimple, di.DockStyle, di.DockWidth, di.Rect, ID_SIMPLE1);
						break;
					case ID_SIMPLE2:
						pDock->AddUndockedChild(new CDockSimple, di.DockStyle, di.DockWidth, di.Rect, ID_SIMPLE2);
						break;
					case ID_TEXT1:
						pDock->AddUndockedChild(new CDockText, di.DockStyle, di.DockWidth, di.Rect, ID_TEXT1);
						break;
					case ID_TEXT2:
						pDock->AddUndockedChild(new CDockText, di.DockStyle, di.DockWidth, di.Rect, ID_TEXT2);
						break;
					default:
						TRACE("Unknown Dock ID\n");
						break;
					}
				}
				else
				{
					if (di.DockParentID)
						pDock = m_DockView.GetDockFromID(di.DockParentID);
					
					switch(di.DockID)
					{
					case ID_CLASS1:
						pDock->AddDockedChild(new CDockClass, di.DockStyle, di.DockWidth, ID_CLASS1);
						break;
					case ID_CLASS2:
						pDock->AddDockedChild(new CDockClass, di.DockStyle, di.DockWidth, ID_CLASS2);					
						break;
					case ID_FILES1:
						pDock->AddDockedChild(new CDockFiles, di.DockStyle, di.DockWidth, ID_FILES1);
						break;
					case ID_FILES2:
						pDock->AddDockedChild(new CDockFiles, di.DockStyle, di.DockWidth, ID_FILES2);
						break;
					case ID_SIMPLE1:
						pDock->AddDockedChild(new CDockSimple, di.DockStyle, di.DockWidth, ID_SIMPLE1);
						break;
					case ID_SIMPLE2:
						pDock->AddDockedChild(new CDockSimple, di.DockStyle, di.DockWidth, ID_SIMPLE2);
						break;
					case ID_TEXT1:
						pDock->AddDockedChild(new CDockText, di.DockStyle, di.DockWidth, ID_TEXT1);
						break;
					case ID_TEXT2:
						pDock->AddDockedChild(new CDockText, di.DockStyle, di.DockWidth, ID_TEXT2);
						break;
					default:
						TRACE("Unknown Dock ID\n");
						break;
					}
				}

				i++;
				tsSubKey = _T("DockChild");
				tsSubKey += _itot(i, szNumber, 10);
			}
		}

		RegCloseKey(hKey);
	}
}

void CMainFrame::SaveDockables()
{
	// NOTE: This function assumes that each dockable has a unique DockID

	std::vector<DockedInfo> vDockList;
	std::vector <CDockable*> & v1 = m_DockView.GetDockChildren();
	std::vector <CDockable*>::iterator itor;

	// Add m_DockView's docked children to the DockList vector
	for (itor = v1.begin(); itor !=  v1.end(); ++itor)
	{
		DockedInfo di = {0};
		di.DockParentID = 0;
		di.DockID = (*itor)->GetDockID();
		di.DockStyle = (*itor)->GetDockStyle();
		di.DockWidth = (*itor)->GetDockWidth();

		if (0 != di.DockID)
			vDockList.push_back(di);
	}

	// Add remaining docked children of children to the DockList vector
	UINT u = 0;
	while (u < vDockList.size())
	{
		//Add all the children of DockList[u]
		CDockable* pDock = m_DockView.GetDockFromID(vDockList[u].DockID);
		std::vector <CDockable*> & v2 = pDock->GetDockChildren();

		for (itor = v2.begin(); itor != v2.end(); ++itor)
		{
			DockedInfo di = {0};
			di.DockParentID = (*itor)->GetDockParent()->GetDockID();
			di.DockID = (*itor)->GetDockID();
			di.DockID = (*itor)->GetDockID();
			di.DockStyle = (*itor)->GetDockStyle();
			di.DockWidth = (*itor)->GetDockWidth();

			if ((0 != di.DockID) && (0 != di.DockParentID))
				vDockList.push_back(di);
		}

		++u;
	}

	TCHAR szText[80];
	wsprintf(szText, _T("# of Docked entries = %d\n"), (int)vDockList.size());
	TRACE(szText);

	if (0 != GetRegistryKeyName().size())
	{
		tString tsKeyName = _T("Software\\") + GetRegistryKeyName();
		HKEY hKey = NULL;
		HKEY hKeyDock = NULL;
		if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		RegDeleteKey(hKey, _T("Docked Windows"));
		if (RegCreateKeyEx(hKey, _T("Docked Windows"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyDock, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		// Add the Docked windows information to the registry
		for (UINT u = 0; u < vDockList.size(); ++u)
		{
			DockedInfo di = vDockList[u];
			TCHAR szNumber[16];
			tString tsSubKey = _T("DockChild");
			tsSubKey += _itot(u, szNumber, 10);
			RegSetValueEx(hKeyDock, tsSubKey.c_str(), 0, REG_BINARY, (LPBYTE)&di, sizeof(DockedInfo));
		}

		// Add the Undocked windows information to the registry
		int nTotal = vDockList.size();
		for (UINT v = 0; v <  m_DockView.GetAllDockables().size(); ++v)
		{
			CDockable* pDock = m_DockView.GetAllDockables()[v];
			if (pDock->IsUndocked())
			{
				DockedInfo di = {0};
				di.DockID = pDock->GetDockID();
				di.DockParentID = -1;
				di.DockStyle = pDock->GetDockStyle();
				di.DockWidth = pDock->GetDockWidth();
				di.Rect = pDock->GetWindowRect();
				TCHAR szNumber[16];
				tString tsSubKey = _T("DockChild");
				tsSubKey += _itot(nTotal, szNumber, 10);
				RegSetValueEx(hKeyDock, tsSubKey.c_str(), 0, REG_BINARY, (LPBYTE)&di, sizeof(DockedInfo));
				++nTotal;
			}
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

