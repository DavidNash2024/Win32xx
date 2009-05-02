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

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Themes Sample"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::AddCombo()
{ 
	// We'll be placing the ComboBoxEx control over the 'File Save' toolbar button
	int nComboWidth = 120; 
	CToolbar& TB = GetToolbar();
	if (TB.CommandToIndex(IDM_FILE_SAVE) < 0) return;
     
	TB.SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);	// Convert the button to a seperator
	TB.SetButtonWidth(IDM_FILE_SAVE, nComboWidth);
	 
	// Determine the size and position of the ComboBox 
	int nIndex = TB.CommandToIndex(IDM_FILE_SAVE); 
	CRect rc = TB.GetItemRect(nIndex); 
	 
	// Create and position the ComboboxEx window 
	m_ComboBoxEx.Create(TB.GetHwnd());
	m_ComboBoxEx.SetWindowPos(NULL, rc, SWP_NOACTIVATE);

	// Set ComboBox Height
	m_ComboBoxEx.SendMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)rc.Height()-6);

	// Resize rebar band to accomodate the new width of the toolbar
	if (IsRebarUsed())
	{
		CRebar& RB = GetRebar();
		int iBand = RB.GetBand(TB.GetHwnd());
		RB.ResizeBand(iBand, TB.GetMaxSize());
	}

	m_ComboBoxEx.AddItems();
	RecalcLayout();
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
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
			if (IsRebarUsed()) MB.SetMenubarTheme(tm);
			SetMenuTheme(tm);

			::CheckMenuRadioItem(hTheme, IDM_NONE, IDM_GOLD, IDM_MAUVE, 0);
		}
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
	// m_bUseToolbar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
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

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the first toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Set the three image lists for the first toolbar
	SetToolbarImages(RGB(255, 0, 255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Set the icons for popup menu items
	SetMenuIcons(GetToolbar().GetToolbarData(), RGB(255, 0, 255), IDB_TOOLBAR_SML, 0);

	// Add the two other toolbars if we use rebars
	if (IsRebarUsed())
	{
		//Set our theme
		ChooseTheme(IDM_BLUE);

		// Add the Arrows toolbar
		AddToolbarBand(Arrows);
		Arrows.AddToolbarButton(IDM_ARROW_LEFT);
		Arrows.AddToolbarButton(IDM_ARROW_RIGHT);
		Arrows.SetImages(RGB(255,0,255), IDB_ARROWS, 0, 0);
		
		// Add the Cards toolbar
		AddToolbarBand(Cards);
		Cards.AddToolbarButton(IDM_CARD_CLUB);
		Cards.AddToolbarButton(IDM_CARD_DIAMOND);
		Cards.AddToolbarButton(IDM_CARD_HEART);
		Cards.AddToolbarButton(IDM_CARD_SPADE);
		Cards.SetImages(RGB(255,0,255), IDB_CARDS, 0, 0);
	}

	AddCombo();
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

