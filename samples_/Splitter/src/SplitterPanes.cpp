//////////////////////////////////////////////
// SplitterPanes.cpp

#include "stdafx.h"
#include "SplitterApp.h"
#include "Mainfrm.h"
#include "SplitterPanes.h"


CDockSimple::CDockSimple() 
{
	SetView(m_View);
}

void CDockSimple::OnInitialUpdate()
{
}

CDockText::CDockText() 
{
	SetView(m_View);
}

void CDockText::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(6);
}


CDockTree::CDockTree() 
{
	SetView(m_View);
}

void CDockTree::OnInitialUpdate() 
{
	// Set the width of the splitter bar
	SetBarWidth(12);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetSplitApp().GetMainFrame();
	CReBar& RB = MainFrame.GetReBar();
	if (RB.GetReBarTheme().UseThemes && RB.GetReBarTheme().clrBkgnd1 != 0)
		SetBarColor(RB.GetReBarTheme().clrBkgnd1);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 
}

CDockList::CDockList() 
{
	SetView(m_View);
}

void CDockList::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(6);
}

