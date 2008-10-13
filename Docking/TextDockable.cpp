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

}


void CTextDockable::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetDockApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 

	SetCaption (_T("Class View - Docking"));

}

