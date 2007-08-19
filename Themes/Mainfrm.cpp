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
	case IDM_DEFAULT_THEME:
		SetTheme(IDM_DEFAULT_THEME);
		return TRUE;
	case IDM_BLUE:
		SetTheme(IDM_BLUE);
		return TRUE;
	case IDM_BLUE_LOCKED:
		SetTheme(IDM_BLUE_LOCKED);
		return TRUE;
	case IDM_BLUE_BKGND:
		SetTheme(IDM_BLUE_BKGND);
		return TRUE;
	case IDM_BLUE_FLAT:
		SetTheme(IDM_BLUE_FLAT);
		return TRUE;
	case IDM_GREY:
		SetTheme(IDM_GREY);
		return TRUE;
	case IDM_OLIVE:
		SetTheme(IDM_OLIVE);
		return TRUE;
	case IDM_OCHRE:
		SetTheme(IDM_OCHRE);
		return TRUE;
	case IDM_MAUVE:
		SetTheme(IDM_MAUVE);
		return TRUE;
	case IDM_PINK:
		SetTheme(IDM_PINK);
		return TRUE;
	case IDM_YELLOW:
		SetTheme(IDM_YELLOW);
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
	
	CToolbar& TB = GetToolbar();
	TB.SetImageList(8, RGB(255, 0, 255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Resize the Rebar band
	if (IsRebarUsed())
	{	
		// Set the icons for popup menu items
		GetMenubar().SetIcons(m_ToolbarData, IDB_TOOLBAR_SML, RGB(255, 0, 255));
		
		// Resize the rebar for the larger toolbar buttons
		CRebar& RB = GetRebar();
		RB.ResizeBand(RB.GetBand(TB.GetHwnd()), TB.GetMaxSize());
	
		//Set our theme
		SetTheme(IDM_BLUE);

		// Add the Arrows toolbar
		std::vector<UINT> ArrowsData;
		ArrowsData.push_back(IDM_ARROW_LEFT);
		ArrowsData.push_back(IDM_ARROW_RIGHT);
		AddToolbarBand(Arrows, ArrowsData, RGB(255,0,255), IDB_ARROWS);

		// Add the Cards toolbar
		std::vector<UINT> CardsData;
		CardsData.push_back(IDM_CARD_CLUB);
		CardsData.push_back(IDM_CARD_DIAMOND);
		CardsData.push_back(IDM_CARD_HEART);
		CardsData.push_back(IDM_CARD_SPADE);
		AddToolbarBand(Cards, CardsData, RGB(255,0,255), IDB_CARDS);
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

void CMainFrame::SetTheme(UINT nStyle)
{
	CRebar& RB = GetRebar();
	CToolbar& TB = GetToolbar();
	BOOL T = TRUE;
	BOOL F = FALSE;
	HMENU hTheme = ::GetSubMenu(GetFrameMenu(), 3);

	switch (nStyle)
	{
	case IDM_DEFAULT_THEME:	// Disable themes
		{
			REBARTHEME rt = {0};
			RB.SetTheme(rt);
			
			TOOLBARTHEME tt = {0};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_DEFAULT_THEME, 0);
		}
		break;
	
	case IDM_BLUE:	// ICY_BLUE Theme
		{			
			REBARTHEME rt = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, F, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_BLUE, 0);
		}
		break;

	case IDM_BLUE_LOCKED:	// ICY_BLUE Theme
		{			
			REBARTHEME rt = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_BLUE_LOCKED, 0);
		}
		break;

	case IDM_BLUE_BKGND:	// ICY_BLUE background only
		{
			REBARTHEME rt = {T, RGB(150,190,245), RGB(196,215,250), NULL, NULL, F, F, F, F, F, T };
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);
			
			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_BLUE_BKGND, 0);
		}
		break;

	case IDM_BLUE_FLAT:	// ICY_BLUE Flat Theme
		{
			REBARTHEME rt = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), T, T, F, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_BLUE_FLAT, 0);
		}
		break;

	case IDM_GREY:	// Grey Theme
		{			
			REBARTHEME rt = {T, RGB(230, 230, 215), RGB(238, 236, 224), RGB(248, 247, 243), RGB(195, 195, 172), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_GREY, 0);
		}
		break;

	case IDM_OLIVE:
		{			
			REBARTHEME rt = {T, RGB(50, 150, 50), RGB(4, 204, 78), RGB(0, 120, 0), RGB(27, 159, 78), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_OLIVE, 0);
		}
		break;

	case IDM_YELLOW:
		{			
			REBARTHEME rt = {T, RGB(160, 160, 94), RGB(216, 216, 40), RGB(255, 255, 255), RGB(183, 183, 46), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_YELLOW, 0);
		}
		break;

	case IDM_OCHRE:
		{			
			REBARTHEME rt = {T, RGB(248, 132, 12), RGB(248, 198, 10), RGB(248, 217, 78), RGB(248, 141, 6), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_OCHRE, 0);
		}
		break;
	case IDM_PINK:
		{			
			REBARTHEME rt = {T, RGB(248, 107, 243), RGB(248, 153, 179), RGB(248, 153, 179), RGB(195, 195, 172), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(248, 147, 220), RGB(248, 153, 179), RGB(49, 106, 197)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_PINK, 0);
		}
		break;
	case IDM_MAUVE:
		{			
			REBARTHEME rt = {T, RGB(190, 100, 125), RGB(210, 128, 155), RGB(219, 138, 188), RGB(90, 100, 125), F, T, T, T, T, F};
			RB.SetTheme(rt);

			TOOLBARTHEME tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetTheme(tt);
			Arrows.SetTheme(tt);
			Cards.SetTheme(tt);

			::CheckMenuRadioItem(hTheme, IDM_DEFAULT_THEME, IDM_YELLOW, IDM_MAUVE, 0);
		}
		break;

	}

	::InvalidateRect(GetMenubar().GetHwnd(), NULL, TRUE);
	RecalcLayout();
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

