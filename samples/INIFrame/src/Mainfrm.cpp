////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_ShowStatusBar(1), m_ShowToolBar(1)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	// Set m_View as the view window of the frame
	SetView(m_View);

	// Normally we would use LoadRegistrySettings here, but this
	// is omitted in this sample. We use an ini file instead.
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSave();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	
	int Res = CFrame::OnCreate(cs);
	ShowStatusBar(m_ShowStatusBar);
	ShowToolBar(m_ShowToolBar);
	return Res;
}

void CMainFrame::OnClose()
{
	SerializeINI(true);
	CFrame::OnClose();
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileOpen()
{
	// Bring up the dialog, and open the file
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	GetDoc().FileLoad(str);
	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	GetDoc().FileStore(str);
	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
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

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
//		Add case statements for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// Load setting from Frame.ini
	SerializeINI(false);	
	
	// Set the frame window styles
	cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	if (GetShowCmd() == SW_MAXIMIZE) cs.style |= WS_MAXIMIZE;

	CWindowDC dcDesktop(0);

	// Does the window fit on the desktop?
	if (RectVisible(dcDesktop, &m_rcPosition) && (m_rcPosition.Width() > 0))
	{
		// Set the original window position
		cs.x  = m_rcPosition.left;
		cs.y  = m_rcPosition.top;
		cs.cx = m_rcPosition.Width();
		cs.cy = m_rcPosition.Height();
	}
}

CString CMainFrame::ItoT(int i)
// Integer to TCHAR. Returns a CString
{
	// tStringStream is a TCHAR std::stringstream
	tStringStream tss;
	tss << i;
	return CString(tss.str().c_str());
}

int CMainFrame::TtoI(LPCTSTR szString)
// TCHAR to Integer.
{
	// tStringStream is a TCHAR std::stringstream
	tStringStream tss(szString);
	int res;
	tss >> res;
	return res;
}

void CMainFrame::SerializeINI(BOOL IsStoring) 
// Load values to, or restore values from the ini file
{
	CString cs("./Frame.ini");
	CString	Key("Frame Settings");

	WINDOWPLACEMENT Wndpl;
	ZeroMemory(&Wndpl, sizeof(WINDOWPLACEMENT));
	Wndpl.length = sizeof(WINDOWPLACEMENT);
	
	if (IsStoring)
	{	
		GetWindowPlacement(Wndpl);

		CRect rc = Wndpl.rcNormalPosition;
		UINT top = MAX(rc.top, 0);
		UINT left = MAX(rc.left, 0);
		UINT width = MAX(rc.Width(), 100);
		UINT height = MAX(rc.Height(), 50);
		UINT showCmd = Wndpl.showCmd;

		::WritePrivateProfileString( NULL, NULL, NULL, cs );

		// Write the Frame window's position and show state 
		::WritePrivateProfileString (Key, _T("Left"),       ItoT(left), cs); 
		::WritePrivateProfileString (Key, _T("Top"),        ItoT(top), cs); 
		::WritePrivateProfileString (Key, _T("Width"),      ItoT(width), cs);
		::WritePrivateProfileString (Key, _T("Height"),     ItoT(height), cs);
		::WritePrivateProfileString (Key, _T("ShowCmd"),    ItoT(showCmd), cs);

		// Write the StatusBar and ToolBar show state.
		DWORD dwShowStatusBar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
		DWORD dwShowToolBar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
		::WritePrivateProfileString (Key, _T("StatusBar"),  ItoT(dwShowStatusBar), cs);
		::WritePrivateProfileString (Key, _T("ToolBar"),    ItoT(dwShowToolBar), cs);
	}
	else 
	{
		UINT failed = 999999;
		CString Error("Error: GPPS failed");
		
		UINT Left = ::GetPrivateProfileInt(Key, _T("Left"), failed, cs);
		UINT Top = ::GetPrivateProfileInt (Key, _T("Top"), failed, cs);
		UINT Width = ::GetPrivateProfileInt (Key, _T("Width"), failed, cs);
		UINT Height = ::GetPrivateProfileInt (Key, _T("Height"), failed, cs);	
		UINT ShowCmd = ::GetPrivateProfileInt (Key, _T("ShowCmd"), failed, cs);

		if (Left != failed && Top != failed && Width != failed && Height != failed && ShowCmd != failed) 
		{
			m_rcPosition = CRect(Left, Top, Left + Width, Top + Height);
			SetShowCmd(ShowCmd);

			// Set the show state of the status bar
			UINT ShowStatus = ::GetPrivateProfileInt (Key, _T("StatusBar"), 0, cs);
			if (ShowStatus != failed)
				m_ShowStatusBar = ShowStatus;

			// Set the show state of the tool bar
			UINT ShowTool = ::GetPrivateProfileInt (Key, _T("ToolBar"), 0, cs);
			if (ShowTool != failed)
				m_ShowToolBar = ShowTool;
		}
		else
		{
			TRACE("Failed to load ini file settings");
			;;
		}
	}
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	
	AddToolBarButton( 0 );						// Separator
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

