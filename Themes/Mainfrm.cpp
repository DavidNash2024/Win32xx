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
	case IDM_NONE:
		ChooseTheme(IDM_NONE);
		return TRUE;
	case IDM_BLUE:
		ChooseTheme(IDM_BLUE);
		return TRUE;
	case IDM_BLUE_LOCKED:
		ChooseTheme(IDM_BLUE_LOCKED);
		return TRUE;
	case IDM_BLUE_BKGND:
		ChooseTheme(IDM_BLUE_BKGND);
		return TRUE;
	case IDM_BLUE_FLAT:
		ChooseTheme(IDM_BLUE_FLAT);
		return TRUE;
	case IDM_GREY:
		ChooseTheme(IDM_GREY);
		return TRUE;
	case IDM_OLIVE:
		ChooseTheme(IDM_OLIVE);
		return TRUE;
	case IDM_OCHRE:
		ChooseTheme(IDM_OCHRE);
		return TRUE;
	case IDM_MAUVE:
		ChooseTheme(IDM_MAUVE);
		return TRUE;
	case IDM_PINK:
		ChooseTheme(IDM_PINK);
		return TRUE;
	case IDM_GOLD:
		ChooseTheme(IDM_GOLD);
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
	SetToolbarImages(TB, 8, RGB(255, 0, 255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Set the icons for popup menu items
	SetMenuIcons(m_ToolbarData, RGB(255, 0, 255), IDB_TOOLBAR_SML, 0);

	//Set our theme
	ChooseTheme(IDM_BLUE);

	// Resize the Rebar band
	if (IsRebarUsed())
	{		
		// Add the Arrows toolbar
		std::vector<UINT> ArrowsData;
		ArrowsData.push_back(IDM_ARROW_LEFT);
		ArrowsData.push_back(IDM_ARROW_RIGHT);
		AddToolbarBand(Arrows);
		int iButtons = Arrows.SetButtons(ArrowsData);
		SetToolbarImages(Arrows, iButtons, RGB(255,0,255), IDB_ARROWS, 0, 0);	

		// Add the Cards toolbar
		std::vector<UINT> CardsData;
		CardsData.push_back(IDM_CARD_CLUB);
		CardsData.push_back(IDM_CARD_DIAMOND);
		CardsData.push_back(IDM_CARD_HEART);
		CardsData.push_back(IDM_CARD_SPADE);
		AddToolbarBand(Cards);
		iButtons = Cards.SetButtons(CardsData);
		SetToolbarImages(Cards, iButtons, RGB(255,0,255), IDB_CARDS, 0, 0);
	}
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
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

void CMainFrame::ChooseTheme(UINT nStyle)
{
	CRebar& RB = GetRebar();
	CToolbar& TB = GetToolbar();
	CMenubar& MB = GetMenubar();
	BOOL T = TRUE;
	BOOL F = FALSE;
	HMENU hTheme = ::GetSubMenu(GetFrameMenu(), 3);

	switch (nStyle)
	{
	case IDM_NONE:	// Disable themes
		{
			ThemeRebar tr = {0};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);
			
			ThemeToolbar tt = {0};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {0};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_NONE, 0);
		}
		break;
	
	case IDM_BLUE:	// ICY_BLUE Theme
		{			
			ThemeRebar tr = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, F, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_BLUE, 0);
		}
		break;

	case IDM_BLUE_LOCKED:	// ICY_BLUE Theme
		{			
			ThemeRebar tr = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_BLUE_LOCKED, 0);
		}
		break;

	case IDM_BLUE_BKGND:	// ICY_BLUE background only
		{
			ThemeRebar tr = {T, RGB(150,190,245), RGB(196,215,250), 0, 0, F, F, F, F, F, T };
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);
			
			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_BLUE_BKGND, 0);
		}
		break;

	case IDM_BLUE_FLAT:	// ICY_BLUE Flat Theme
		{
			ThemeRebar tr = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), T, T, F, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_BLUE_FLAT, 0);
		}
		break;

	case IDM_GREY:	// Grey Theme
		{			
			ThemeRebar tr = {T, RGB(210, 210, 200), RGB(238, 236, 224), RGB(248, 247, 243), RGB(195, 195, 172), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);
	
			ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(210, 210, 200), RGB(248, 247, 243), RGB(128, 128, 200)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_GREY, 0);
		}
		break;

	case IDM_OLIVE:
		{			
			ThemeRebar tr = {T, RGB(160, 180, 80), RGB(180, 200, 100), RGB(200, 220, 120), RGB(80, 159, 78), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 160, 50), RGB(255, 210, 90), RGB(128, 128, 128)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_OLIVE, 0);
		}
		break;

	case IDM_GOLD:
		{			
			ThemeRebar tr = {T, RGB(230, 180, 0), RGB(240, 210, 90), RGB(255, 240, 150), RGB(180, 140, 50), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(240, 210, 90), RGB(255, 240, 150), RGB(128, 128, 128)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_GOLD, 0);
		}
		break;

	case IDM_OCHRE:
		{			
			ThemeRebar tr = {T, RGB(248, 132, 12), RGB(248, 198, 10), RGB(248, 210, 20), RGB(248, 141, 6), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 128)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_OCHRE, 0);
		}
		break;
	case IDM_PINK:
		{			
			ThemeRebar tr = {T, RGB(255, 130, 190), RGB(250, 205, 235), RGB(250, 205, 235), RGB(255, 120, 170), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(248, 147, 220), RGB(248, 153, 179), RGB(49, 106, 197)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(255, 130, 190), RGB(250, 205, 235), RGB(128, 128, 128)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_PINK, 0);
		}
		break;
	case IDM_MAUVE:
		{			
			ThemeRebar tr = {T, RGB(210, 128, 155), RGB(230, 160, 190), RGB(230, 158, 188), RGB(110, 100, 125), F, T, T, T, T, F};
			if (IsRebarUsed()) RB.SetRebarTheme(tr);

			ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
			TB.SetToolbarTheme(tt);
			Arrows.SetToolbarTheme(tt);
			Cards.SetToolbarTheme(tt);

			ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 128)};
			MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_MAUVE, 0);
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

