////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
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

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
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
	return CFrame::OnCreate(pcs);
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

 //   typedef CStringT<WCHAR> CStringX;
	typedef CString CStringX;
 //   typedef CStringT<char> CStringX;

	CStringX a1(L'g');
	CStringX a2(L'h', 20);
	CStringX a3(L"Some Text");
	CStringX a4(L"Some Text", 35);
	CStringX a5(a1);
	CStringX a6 = a1;
	CStringX a7 = L"Some Text";

	if (a3 == a2)
        TRACE("a3 ==a2\n");
	if (a1 == a2)
        TRACE("a1 == a2\n");
	if (a3 == _T("Some Text") )
        TRACE("a3 == Some Text\n");
	if (a2 < a3)
	    TRACE("a2 < a3");


	CStringX a8;
	a8.Format(_T(" A count of songs %d"), 8);

	CStringX a9;
	a9.GetErrorString(33);
	a9 = a9 + " More Text";
	a9 += L" And even some more text";

	int len = GetWindowTextLength();
	CStringX a10;
	::GetWindowText(m_hWnd, a10.GetBuffer(len), len+1);
	a10.ReleaseBuffer();

	BSTR bstr10 = a10.AllocSysString();
	BSTR bstr9 = a9.SetSysString(&bstr10);

	SysFreeString(bstr9);
//	SysFreeString(bstr10);

	int i = a9.Collate(_T("More Text"));
	i = a9.CollateNoCase(_T("More Text"));
	i = a9.Compare(_T("More Text"));
	i = a9.CompareNoCase(_T("More Text"));
	a9.Delete(5, 3);
	a9.Empty();
	a9 = L"Some Text";
	i = a9.Find(_T("Text"));
	i = a9.Find(_T('t'));
	i = a9.FindOneOf(_T("abcde"));
	WCHAR ch = a9.GetAt(i);
	a9 += ch;
 	a9.GetEnvironmentVariable(_T("PUBLIC"));
 	i = a9.GetLength();

    std::basic_string<TCHAR> s9 = a9.GetString();
    a9.Insert(3, _T('j'));
    a9.Insert(4, _T("XXXXXX"));
    if (!a9.IsEmpty())
        TRACE("a9 not empty");

    a1 = a9.Left(5);
    a2.LoadString(IDW_MAIN);
    a9.MakeLower();
    a9.MakeReverse();
    a9.MakeUpper();
    a1 = a9.Mid(3);
    a1 = a9.Mid(4, 4);
    CStringX a11(a9.Mid(4, 4));
    CStringX a12 = a9.Mid(4, 4);
    CStringX a13 = a9.Left(5);
    a9 = L"Here is some text";
    a9.Remove(_T("some "));
    a9.Replace(_T("text"), _T("tttt"));
    a9.Replace(_T('t'), _T('s'));
    i = a9.ReverseFind(_T("is"));
    a1 = a9.Right(6);
    a9.SetAt(5, 'X');
    a1 = a9.SpanExcluding(_T("te"));
    a2 = a9.SpanIncluding(_T("te"));

    a9= L"A1 a2 aaa3 a4 a9";

    i = 0;
    a3 = a9.Tokenize(_T(" "), i);
    a3 = a9.Tokenize(_T(" "), i);
    a3 = a9.Tokenize(_T(" "), i);
    a4 = a9.Tokenize(_T(" "), i);

    a10 = L"   XXXYYZZ      ";
    a9 = a10;
    a8 = a10;
    a8.Trim();
    a9.TrimLeft();
    a10.TrimRight();
    a9.Truncate(8);

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
	// This function is called before the frame is created.
	// It provides an opportunity to modify the various CREATESTRUCT
	// parameters used in the frame window's creation.

	// The WS_EX_LAYOUTRTL style requires Windows 2000 or above in targetver.h
	// cs.dwExStyle = WS_EX_LAYOUTRTL;		// Set Right-To-Left Window Layout

	// Call base clase to set defaults
	CFrame::PreCreate(cs);

	// cs.style &= ~WS_VISIBLE;	// Remove the WS_VISIBLE style. The frame will be initially hidden.
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

