////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the Resource IDs for the toolbar buttons
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_FILE_NEW   );
	m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_EDIT_CUT   );
	m_ToolbarData.push_back ( IDM_EDIT_COPY  );
	m_ToolbarData.push_back ( IDM_EDIT_PASTE );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_FILE_PRINT );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HELP_ABOUT );
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::AddToolbar(CToolbar& TB, std::vector<UINT> TBData, UINT ID_Normal)
{
	if (TBData.size() == 0)
	{
		DebugErrMsg(_T("Toolbar must have some data"));
		return;
	}

	// Create the Toolbar Window
	TB.Create(GetRebar().GetHwnd());

	TB.SetImageList((int)TBData.size(), RGB(255,0,255), ID_Normal, 0, 0);
	TB.SetButtons(TBData);

	// Fill the REBARBAND structure
	REBARBANDINFO rbbi = {0};
	SIZE sz = TB.GetMaxSize();

	rbbi.cbSize     = sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE |  RBBIM_CHILD | RBBIM_SIZE;
	rbbi.cyMinChild = sz.cy;
	rbbi.cyMaxChild = sz.cy;
	rbbi.cx         = sz.cx;
	rbbi.cxMinChild = sz.cx;

	rbbi.fStyle     = /*RBBS_BREAK |*/ RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.hwndChild  = TB.GetHwnd();

	GetRebar().InsertBand(-1, &rbbi);
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

	

	// call the base class function
	CFrame::OnCreate();
	SetTheme(1);
//	SetTheme(2);
//	SetTheme(0);

	if (IsRebarUsed())
	{
		// Add the Arrows toolbar
		std::vector<UINT> ArrowsData;
		ArrowsData.push_back(IDM_ARROW_LEFT);
		ArrowsData.push_back(IDM_ARROW_RIGHT);
		AddToolbar(Arrows, ArrowsData, IDB_ARROWS);

		// Add the Cards toolbar
		std::vector<UINT> CardsData;
		CardsData.push_back(IDM_CARD_CLUB);
		CardsData.push_back(IDM_CARD_DIAMOND);
		CardsData.push_back(IDM_CARD_HEART);
		CardsData.push_back(IDM_CARD_SPADE);
		AddToolbar(Cards, CardsData, IDB_CARDS);
	}
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created");
}

LRESULT CMainFrame::OnNotify(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
 		//Add case statments for each notification message here
//	}

	// Pass any unhandled messages on for default processing
	return 0L;
}

void CMainFrame::SetButtons(const std::vector<UINT> ToolbarData)
{
	// Overriding CFrame::Setbuttons is optional. We do it here to use larger buttons
	// with seperate imagelists for normal, hot and disabled buttons.

	// A reference to the CToolbar object
	CToolbar& TB = GetToolbar();

	// Set the button size to 24x24 before adding the bitmap
	TB.SetBitmapSize(24, 24);

	if (IsRebarUsed())
	{
		// Set the image lists for normal, hot and disabled buttons from full colour bitmaps
		TB.SetImageList(8, RGB(255,0,255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);
	}
	else
	{
		// Use a 256 colour bitmap
		TB.AddBitmap(8, IDW_MAIN);
	}

	// Set the resource IDs for the toolbar buttons
	TB.SetButtons(ToolbarData);

	// Adjust the toolbar and rebar size to take account of the larger buttons
	RECT r;
	TB.GetItemRect(TB.CommandToIndex(IDM_HELP_ABOUT), &r);
	TB.SetButtonSize(r.right - r.left, r.bottom - r.top);

	// Disable some of the toolbar buttons
	TB.DisableButton(IDM_EDIT_CUT);
	TB.DisableButton(IDM_EDIT_COPY);
	TB.DisableButton(IDM_EDIT_PASTE);

	TB.SetButtonText(IDM_FILE_OPEN, _T("Open"));

	// Use smaller icons for popup menu items
	GetMenubar().SetIcons(m_ToolbarData, IDB_TOOLBAR_SML, RGB(255, 0, 255));
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

