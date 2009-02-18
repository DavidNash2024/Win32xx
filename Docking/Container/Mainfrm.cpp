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
		SetRedraw(FALSE);	// Suppress drawing to the frame window
		m_DockView.CloseAllDockables();
		LoadDefaultDockables();
		SetRedraw(TRUE);	// Re-enable drawing to the frame window
		RedrawWindow(0, 0, RDW_INVALIDATE|RDW_FRAME|RDW_UPDATENOW|RDW_ALLCHILDREN);
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
	
	// Add the remaining dockables
	CDockable* pDockRight  = m_DockView.AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | dwStyle, 200, ID_CONTAINCLASSES1);	
	CDockable* pDockBottom = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_TEXT);

	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINFILES1);
	pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINCLASSES2);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINFILES2);

	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_OUTPUT);
}

void CMainFrame::LoadRegistryDockables()
{
	if (0 != GetRegistryKeyName().size())
	{
		LPCTSTR szSubKeyDock = _T("Dock Windows");
		tString tsKey = _T("Software\\") + GetRegistryKeyName() + _T("\\") + szSubKeyDock;
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
				if (-1 == di.DockParentID)
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
					case ID_OUTPUT:
						m_DockView.AddUndockedChild(new CDockOutput, di.DockStyle, di.DockWidth, di.Rect, ID_OUTPUT);
						break;
					case ID_TEXT:
						m_DockView.AddUndockedChild(new CDockText, di.DockStyle, di.DockWidth, di.Rect, ID_TEXT);
						break;
					default:
						TRACE("Unknown Dock ID\n");
						break;
					}
				}
				else
				{
					CDockable* pDock = 0;	
					if (di.DockParentID)
						pDock = m_DockView.GetDockFromID(di.DockParentID);
					
					if (0 == pDock)
						pDock = &m_DockView;
					
					if (pDock)
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
						case ID_OUTPUT:
							pDock->AddDockedChild(new CDockOutput, di.DockStyle, di.DockWidth, ID_OUTPUT);
							break;
						case ID_TEXT:
							pDock->AddDockedChild(new CDockText, di.DockStyle, di.DockWidth, ID_TEXT);
							break;
						default:
							TRACE("Unknown Dock ID\n");
							break;
						}
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
	std::vector <CDockable*>::iterator iter;

	// Add m_DockView's docked children to the DockList vector
	for (iter = v1.begin(); iter !=  v1.end(); ++iter)
	{
		DockedInfo di = {0};
		di.DockParentID = 0;
		di.DockID = (*iter)->GetDockID();
		di.DockStyle = (*iter)->GetDockStyle();
		di.DockWidth = (*iter)->GetDockWidth();

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

		for (iter = v2.begin(); iter != v2.end(); ++iter)
		{
			DockedInfo di = {0};
			di.DockParentID = (*iter)->GetDockParent()->GetDockID();
			di.DockID = (*iter)->GetDockID();
			di.DockID = (*iter)->GetDockID();
			di.DockStyle = (*iter)->GetDockStyle();
			di.DockWidth = (*iter)->GetDockWidth();

			if ((0 != di.DockID) && (0 != di.DockParentID))
				vDockList.push_back(di);
		}

		++u;
	}

	// Add the Undocked windows information to the DockList vector
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
			vDockList.push_back(di);
		}
	}

	// Add container children to the DockList vector
	u = 0;
	UINT uMax = vDockList.size();
	while (u < uMax)
	{
		CDockable* pDock = m_DockView.GetDockFromID(vDockList[u].DockID);
		if (pDock->GetView()->IsContainer())
		{
			std::vector<ContainerInfo>& v3 = ((CContainer*)pDock->GetView())->GetAllContainers();
			std::vector<ContainerInfo>::iterator iter3;
			for (iter3 = v3.begin()+1; iter3 != v3.end(); ++iter3)
			{
				CDockable* pDockContainerChild = pDock->GetDockFromView((*iter3).pContainer);
				DockedInfo di = {0};
				di.DockParentID = pDock->GetDockID();
				di.DockID = pDockContainerChild->GetDockID();
				di.DockStyle = pDockContainerChild->GetDockStyle();
				di.DockWidth = pDockContainerChild->GetDockWidth();
				vDockList.push_back(di);
			}
		}
		++u;
	}

	if (0 != GetRegistryKeyName().size())
	{
		tString tsKeyName = _T("Software\\") + GetRegistryKeyName();
		HKEY hKey = NULL;
		HKEY hKeyDock = NULL;
		if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		LPCTSTR szSubKeyDock = _T("Dock Windows");
		RegDeleteKey(hKey, szSubKeyDock);
		if (RegCreateKeyEx(hKey, szSubKeyDock, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyDock, NULL))
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

		RegCloseKey(hKeyDock);
		RegCloseKey(hKey);
	}
}

void CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();
	SaveDockables();
}


