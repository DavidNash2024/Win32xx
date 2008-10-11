/////////////////////////////////////////////
// TextDockable.cpp

#include "DockingApp.h"
#include "Mainfrm.h"
#include "TextDockable.h"


//////////////////////////////////////////////
//  Definitions for the CTextView class
CTextView::CTextView()
{
}

void CTextView::PreCreate(CREATESTRUCT &cs)
{
	// Set the Window Class name to an edit control
	cs.lpszClass = _T("Edit");
	cs.style = WS_VISIBLE | WS_CHILD | ES_MULTILINE;
}


//////////////////////////////////////////////
//  Definitions for the CTextDockable class
CTextDockable::CTextDockable()
{
	// Set the view window to our edit control
	SetView(m_View);

	// Set the width of the splitter bar
	SetBarWidth(8);
}


void CTextDockable::OnCreate()
{
	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetDockApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 

	// Call the base function
	CDockable::OnCreate();
}

