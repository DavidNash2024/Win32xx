//////////////////////////////////////////////
// SplitterPanes.cpp

#include "stdafx.h"
#include "SplitterApp.h"
#include "Mainfrm.h"
#include "SplitterPanes.h"


CPaneTopLeft::CPaneTopLeft() 
{
	SetView(m_View);
}

void CPaneTopLeft::OnInitialUpdate()
{
}

CPaneTopRight::CPaneTopRight() 
{
	SetView(m_View);
}

void CPaneTopRight::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(6);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetSplitApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 
}


CPaneBottomLeft::CPaneBottomLeft() 
{
	SetView(m_View);
}

void CPaneBottomLeft::OnInitialUpdate() 
{
	// Set the width of the splitter bar
	SetBarWidth(12);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetSplitApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd1);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 
}

CPaneBottomRight::CPaneBottomRight() 
{
	SetView(m_View);
}

void CPaneBottomRight::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(6);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetSplitApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 
}

