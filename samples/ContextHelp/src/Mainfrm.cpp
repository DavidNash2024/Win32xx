////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include <io.h>
#include "resource.h"
#include "App.h"
#include "mainfrm.h"


// A global function to provide access to the CDoc class
CDoc& GetDoc()
{
	return TheApp().GetMainFrame().GetDoc();
}

// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_View(IDD_DIALOG1), m_IsChoosingTopic(FALSE)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	// Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\SdiDocViewForm"));


    ////////////////////////////////////////////////////////
	// Set the path to the chm help file for m_AppHelp
	
	// extract the app name, directory, and  path names
    CString AppPath;
	::GetModuleFileName(NULL, AppPath.GetBuffer(FILENAME_MAX), FILENAME_MAX);
	AppPath.ReleaseBuffer();
	CFile f; // no file opened here, just using the name parsing parts
	f.SetFilePath(AppPath);
	CString AppTitle = f.GetFileTitle();
	CString AppName  = f.GetFileNameWOExt();

	// make the help file path name
	CString HelpDir  = MakeAppDataPath(LoadString(IDS_DATAPATH_SUBDIR) + AppName);
	CString HelpPath = HelpDir + _T("\\") + AppName + LoadString(IDS_HELP_FILE_EXT);

	if (::PathFileExists(HelpPath))
	{
		// communicate help file name to AppHelp object
		m_AppHelp.SetHelpFilePath(HelpPath);
	}

	/////////////////////////////////////////////////
	// generate about box credits (PUT YOUR OWN HERE)

	// make Win32++ version string
	UINT ver = _WIN32XX_VER;
	CString sWin32Version;
	sWin32Version.Format(_T("Win32++ Version %d.%d.%d"), ver / 0x100,
		(ver % 0x100) / 0x10, (ver % 0x10));

	// generate compiler information for the About box
	CString sCompiler;
#ifdef __GNUC__
	sCompiler.Format(_T("Gnu C++ %d.%d.%d"), __GNUC__, __GNUC_MINOR__,
		__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
	sCompiler.Format(_T("MS C++ %d.%d"), _MSC_VER / 100, _MSC_VER % 100);
#else
	sCompiler = _T("(unknown compiler name)");
#endif

	CString sAboutBoxInfo;
	CString date = __DATE__;
	sAboutBoxInfo.Format(_T("%s\n\n(%s.exe)\n%s\n%s\ncompiled with ")
		_T("%s on %s"), LoadString(IDW_MAIN).c_str(), AppName.c_str(),
		LoadString(IDS_APP_VERSION).c_str(), sWin32Version.c_str(),
		sCompiler.c_str(), date.c_str());

	m_HelpAbout.SetCredits(sAboutBoxInfo);
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::ChooseHelpTopic()
{
	m_IsChoosingTopic = TRUE;
	::SetCursor(::LoadCursor(NULL, IDC_HELP));
	SetCapture();
}

UINT CMainFrame::GetIDFromCursorPos()
// Identifies the window from the cursor position and returns its ID
{
	UINT nID = 0;
	CPoint pt = GetCursorPos();
	HWND hWndCtrl = WindowFromPoint(pt);

	if (hWndCtrl == GetToolBar().GetHwnd())
	{
		// Over the toolbar window, so identify the toolbar button

		int nButton = GetToolBar().HitTest();
		nID = GetToolBar().GetCommandID(nButton);
	}
	else if (hWndCtrl == GetHwnd())
	{
		// Over the frame window. Check to see if we are over a non-client area spot.

		// The WM_NCHITTEST message returns a value indicating the position of the
		//  cursor hot spot within the non-client area.
		LRESULT lr = SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));

		nID = IDFR_NCFRAME + lr; // As defined in resource.h
	}
	else if (hWndCtrl != 0)
	{
		// The view window (dialog), dialog controls, menubar and statusbar all have control IDs.
		nID = ::GetDlgCtrlID(hWndCtrl);
	}

	return nID;
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR szKeyName)
{
	CFrame::LoadRegistrySettings(szKeyName);
	GetDoc().LoadDocRegistry(GetRegistryKeyName().c_str());

	return TRUE;
}

CString CMainFrame::MakeAppDataPath(const CString& subpath)
  // Return a string consisting of the APPDATA environmental path with the
  // given subpath appended.  Create this path if it does not exist. If
  // an error is encountered, throw a user exception.
{
	::SetLastError(0);
	CString app_data_path = GetAppDataPath();

	int from = 0;
	int to = subpath.GetLength();
	while (from < to)
	{
		int nextbk = subpath.Find(_T("\\"), from);
		int nextfwd = subpath.Find(_T("/"), from);
		int next = MAX(nextbk, nextfwd);
		if (next < 0)
			next = to;

		CString add = subpath.Mid(from, next - from);
		app_data_path += _T("\\") + add;
		if (!SUCCEEDED(::CreateDirectory(app_data_path, 0)))
		{
			CString msg = app_data_path + _T("\nDirectory creation error.");
			throw CUserException(msg);
		}

		from = ++next;
	}

	return app_data_path;
}

void CMainFrame::NotImplemented() const
{
	::MessageBox(NULL, _T("Feature not implemented."),
		_T("Information"), MB_OK | MB_ICONINFORMATION |
		MB_TASKMODAL);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UINT nID = LOWORD(wParam);

	switch(nID)
	{
	case IDM_FILE_NEW:       NotImplemented();      return TRUE;
	case IDM_FILE_OPEN:      NotImplemented();      return TRUE;
	case IDM_FILE_SAVE:      NotImplemented();      return TRUE;
	case IDM_FILE_SAVEAS:    NotImplemented();      return TRUE;
	case IDM_FILE_PRINT:     NotImplemented();      return TRUE;
	case IDM_FILE_PRINT_PREVIEW: NotImplemented();  return TRUE;
	case IDM_FILE_CLOSE:     NotImplemented();      return TRUE;
	
	case IDM_EDIT_UNDO:      NotImplemented();      return TRUE;
	case IDM_EDIT_REDO:      NotImplemented();      return TRUE;
	case IDM_EDIT_CUT:       NotImplemented();      return TRUE;
	case IDM_EDIT_COPY:      NotImplemented();      return TRUE;
	case IDM_EDIT_PASTE:     NotImplemented();      return TRUE;
	case IDM_EDIT_FIND:      NotImplemented();      return TRUE;
	case IDM_EDIT_REPLACE:   NotImplemented();      return TRUE;
	case IDM_EDIT_DELETE:    NotImplemented();      return TRUE;
	
	case IDM_FONT_CHOICE:    NotImplemented();      return TRUE;
	case IDM_COLOR_CHOICE:   NotImplemented();      return TRUE;

	case ID_CHECK_A:		m_View.OnCheckA();		return TRUE;
	case ID_CHECK_B:		m_View.OnCheckB();		return TRUE;
	case ID_CHECK_C:		m_View.OnCheckC();		return TRUE;
	
	case IDM_FILE_EXIT:		OnFileExit();			return TRUE;
	case IDW_VIEW_STATUSBAR: OnViewStatusBar();		return TRUE;
	case IDW_VIEW_TOOLBAR:	OnViewToolBar();		return TRUE;
	
	case ID_RADIO_A:
	case ID_RADIO_B:		// intentionally blank
	case ID_RADIO_C:		m_View.OnRangeOfIds_Radio(nID - ID_RADIO_A);
		return TRUE;
				 
	case IDM_HELP_ABOUT:	m_HelpAbout.DoModal();		return TRUE;	// Menu item
	case IDM_HELP_CONTENT:	ShowHelpTopic(nID);		return TRUE;	// Menu item
	case IDM_HELP_CONTEXT:	ChooseHelpTopic(); 		return TRUE;	// Toolbar Button
	case IDM_SHIFT_F1:		OnShiftF1();			return TRUE;	// Accelerator
	case IDM_F1:			OnF1();					return TRUE;	// Accelerator
	
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE); // Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);	 // Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);		 // Don't use a ReBar
	// SetUseThemes(FALSE);		 // Don't use themes
	// SetUseToolBar(FALSE);	 // Don't use a ToolBar

	// call the base class function
	int Res = CFrame::OnCreate(cs);
	
	// Add control IDs for various windows
	GetView().SetWindowLongPtr(GWLP_ID, IDD_DIALOG1);
	GetMenuBar().SetWindowLongPtr(GWLP_ID, IDW_MENUBAR);
	GetStatusBar().SetWindowLongPtr(GWLP_ID, IDW_STATUSBAR);

	return Res;
}

void CMainFrame::OnF1()
// Called when the F1 key is pressed
{
	UINT nID = GetIDFromCursorPos();

	if (nID != 0)
		m_AppHelp.ShowHelpTopic(nID);
	else
		m_AppHelp.ShowHelpTopic(_T("Introduction"));
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.
	
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
	// Update the check state of the various menu items
	switch (nID)
	{
	case ID_CHECK_A:	OnUpdateCheckA(nID);	break;
	case ID_CHECK_B:	OnUpdateCheckB(nID);	break;
	case ID_CHECK_C:	OnUpdateCheckC(nID);	break;
	}

	if ((nID >= ID_RADIO_A) && (nID <= ID_RADIO_C))
		OnUpdateRangeOfIds_Radio(nID);

	CFrame::OnMenuUpdate(nID);
}

LRESULT CMainFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Handle the left mouse button click.
// Note: When the frame has mouse capture, clicks over child windows are processed here too.
{
	if (m_IsChoosingTopic)
	{
		UINT nID = GetIDFromCursorPos();
		ShowHelpTopic(nID);
		ReleaseCapture();
		return 0L;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Modify the cursor when appropriate
{
	if (m_IsChoosingTopic)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HELP));
		return TRUE;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

void CMainFrame::OnShiftF1()
// Called when the F1 key is pressed while SHIFT is held down
{
	ChooseHelpTopic();
}

void CMainFrame::OnUpdateCheckA(UINT nID)
{
	BOOL bCheck = GetDoc().GetCheckA();
	GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckB(UINT nID)
{
	BOOL bCheck = GetDoc().GetCheckB();
	GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckC(UINT nID)
{
	BOOL bCheck = GetDoc().GetCheckC();
	GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateRangeOfIds_Radio(UINT nID)
{
	UINT adjId = nID - ID_RADIO_A;
	UINT curRadio = GetDoc().GetRadio();
	BOOL bCheck = (curRadio == adjId);
	int nFileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
	CMenu RadioMenu = GetFrameMenu().GetSubMenu(nFileItem);
	if (bCheck)  
		RadioMenu.CheckMenuRadioItem(ID_RADIO_A, ID_RADIO_C, nID, 0);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	// Call base class function first
	CFrame::PreCreate(cs);

	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_MINIMIZEBOX;
	cs.dwExStyle |= WS_EX_CONTEXTHELP;
}

BOOL CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();
	GetDoc().SaveDocRegistry(GetRegistryKeyName().c_str());

	return TRUE;
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons: start out with a
	// separator just to give some space at the left of the toolbar
	AddToolBarButton(0);  // Separator
	
	// Connect button IDs to button icons, show enabled status, and
	// give the explicit image index iImage of each button in the bitmap.
	// Add the toolbar buttons in the order they are to appear at runtime.
	AddToolBarButton(IDM_FILE_NEW,      TRUE, 0, 0);
	AddToolBarButton(IDM_FILE_OPEN,     TRUE, 0, 1);
	AddToolBarButton(IDM_FILE_SAVE,     TRUE, 0, 2);
	AddToolBarButton(IDM_FILE_SAVEAS,   TRUE, 0, 3);
	AddToolBarButton(IDM_FILE_CLOSE,    TRUE, 0, 4);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_EDIT_CUT,      TRUE, 0, 5);
	AddToolBarButton(IDM_EDIT_COPY,     TRUE, 0, 6);
	AddToolBarButton(IDM_EDIT_PASTE,    TRUE, 0, 7);
	AddToolBarButton(IDM_EDIT_DELETE,   TRUE, 0, 8);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_FILE_PRINT_PREVIEW, TRUE, 0, 9);
	AddToolBarButton(IDM_FILE_PRINT,    TRUE, 0, 10);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_EDIT_UNDO,     TRUE, 0, 11);
	AddToolBarButton(IDM_EDIT_REDO,     TRUE, 0, 12);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_FONT_CHOICE,   TRUE, 0, 13);
	AddToolBarButton(IDM_COLOR_CHOICE,  TRUE, 0, 14);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_EDIT_FIND,     TRUE, 0, 15);
	AddToolBarButton(IDM_EDIT_REPLACE,  TRUE, 0, 16);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_HELP_CONTENT,  TRUE, 0, 17);
	AddToolBarButton(IDM_HELP_CONTEXT,  TRUE, 0, 18);
	AddToolBarButton(0);  // Separator
	  
	// Set the toolbar image list: use defaults for hot and disabled
	SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR_NORM, 0, 0);
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called for a System Command such as SC_CLOSE, SC_CONTEXTHELP etc.
{
	if (wParam == SC_CONTEXTHELP)
		ChooseHelpTopic();

	// Now call CFrame's OnSysCommand
	return CFrame::OnSysCommand(uMsg, wParam, lParam);
}

void CMainFrame::ShowHelpTopic(UINT nID)
// Display the context help for the specified topic
{
	m_AppHelp.ShowHelpTopic(nID);
	m_IsChoosingTopic = FALSE;
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETCURSOR:		return OnSetCursor(uMsg, wParam, lParam);			// Modify the cursor when appropriate
	case WM_SYSCOMMAND:		return OnSysCommand(uMsg, wParam, lParam);			// Add SC_CONTEXTHELP support
	case WM_HELP:			return 0L;											// Suppress default handling on F1 and SHIFT F1
	case WM_LBUTTONDOWN:	return OnLButtonDown(uMsg, wParam, lParam);			// Handle left mouse click
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}


