////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "RibbonUI.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_uRibbonHeight(0)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Frame"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

CRect CMainFrame::GetViewRect() const
{
	// Get the frame's client area
	CRect rcFrame = GetClientRect();

	// Get the statusbar's window area
	CRect rcStatus;
	if (GetStatusbar().IsWindowVisible() || !IsWindowVisible())
		rcStatus = GetStatusbar().GetWindowRect();

	// Get the top rebar or toolbar's window area
	CRect rcTop;
	if (IsRebarSupported() && m_bUseRebar)
		rcTop = GetRebar().GetWindowRect();
	else
		if (GetToolbar().IsWindowVisible())
			rcTop = GetToolbar().GetWindowRect();

	// Return client size less the rebar and status windows
	int top = rcFrame.top + rcTop.Height() + m_uRibbonHeight;
	int left = rcFrame.left;
	int right = rcFrame.right;
	int bottom = rcFrame.Height() - (rcStatus.Height());
	if ((bottom <= top) ||( right <= left))
		top = left = right = bottom = 0;

	CRect rcView(left, top, right, bottom);
	return rcView;
} 

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;
	case IDM_FILE_SAVE:
		// Refer to the tutorial for an example of OnFileSave()
		return TRUE;
	case IDM_FILE_SAVEAS:
		// Refer to the tutorial for an example of OnFileSaveAs()
		return TRUE;
	case IDM_FILE_PRINT:
		OnFilePrint();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	}

	return FALSE;
}

/*void CMainFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolbar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
}
*/

void CMainFrame::OnCreate()
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	TRACE(_T("OnCreate\n"));
	if (GetWinVersion() >= 2601)
	{
		m_bUseRebar = FALSE;				// Don't use rebars
		m_bUseToolbar = FALSE;			// Don't use a toolbar
	}

	CFrame::OnCreate();
	int ver = GetWinVersion();

	if (GetWinVersion() >= 2601)
	{
		if (m_Ribbon.CreateRibbon(this))
			TRACE(_T("Ribbon Created Succesfully\n"));
		else
			TRACE(_T("Failed to create ribbon\n"));
	}
}

void CMainFrame::OnDestroy()
{
	if (GetWinVersion() >= 2601)
	{
		m_Ribbon.DestroyRibbon();
	}

	// End the application when the window is destroyed
	::PostQuitMessage(0);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE(_T("Frame created\n"));
}

void CMainFrame::OnFileOpen()
{
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Open File");

	// Bring up the dialog, and open the file
	::GetOpenFileName(&ofn);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

void CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd = {0};
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
 		//Add case statments for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

HRESULT CMainFrame::RibbonExecute(UINT nCmdID, UINT verb, LPCVOID key, LPCVOID ppropvarValue, LPVOID pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);

	// Use the following casts for void pointers:
	//   const PROPERTYKEY* key
	//   const PROPVARIANT* ppropvarValue
	//   IUISimplePropertySet* pCommandExecutionProperties

	HRESULT hr = S_OK;
	switch(nCmdID)
	{
	case cmdButton1:
		TRACE(_T("Button 1\n"));
		break;
	case cmdButton2:
		TRACE(_T("Button 2\n"));
		break;
	case cmdButton3:
		TRACE(_T("Button 3\n"));
		break;
	case cmdButton4:
		TRACE(_T("Button 4\n"));
		break;
	case cmdButton5:
		TRACE(_T("Button 5\n"));
		break;
	case cmdButton6:
		TRACE(_T("Button 6\n"));
		break;
	case cmdToggleButton1:
		TRACE(_T("Toggle Button 1\n"));
		break;
	case cmdToggleButton2:
		TRACE(_T("Toggle Button 2\n"));
		break;
	case IDC_CMD_EXIT:
		TRACE(_T("Exit button\n"));
		break;
	default:
		TRACE(_T("Unknown button\n"));
		break;
	}

	return hr; 
}

HRESULT CMainFrame::RibbonOnViewChanged(UINT viewId, UINT typeId, void* pView, UINT verb, INT uReasonCode)
{
	UNREFERENCED_PARAMETER(viewId);
	UNREFERENCED_PARAMETER(uReasonCode);

	HRESULT hr = E_NOTIMPL;

	// Checks to see if the view that was changed was a Ribbon view.
	if (UI_VIEWTYPE_RIBBON == typeId)
	{
		switch (verb)
		{           
			// The view was newly created.
		case UI_VIEWVERB_CREATE:
			hr = S_OK;
			break;

			// The view has been resized.  For the Ribbon view, the application should
			// call GetHeight to determine the height of the ribbon.
		case UI_VIEWVERB_SIZE:
			{
				IUIRibbon* pRibbon = NULL;
				UINT uRibbonHeight;

				hr = ((IUnknown*)pView)->QueryInterface(IID_PPV_ARGS(&pRibbon));
				if (SUCCEEDED(hr))
				{
					// Call to the framework to determine the desired height of the Ribbon.
					hr = pRibbon->GetHeight(&uRibbonHeight);
					m_uRibbonHeight = uRibbonHeight;
					pRibbon->Release();

					RecalcLayout();
					// Use the ribbon height to position controls in the client area of the window.
				}
			}
			break;
			// The view was destroyed.
		case UI_VIEWVERB_DESTROY:
			hr = S_OK;
			break;
		}
	}  

	return hr; 
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolbarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolbarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_PRINT );
	
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );
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

