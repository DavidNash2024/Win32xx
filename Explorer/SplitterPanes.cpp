////////////////////////////////////////////////////////
// SplitterPanes.cpp
//  Definitions for the CLeftPane and CRightPane classes

#include "ExplorerApp.h"
#include "SplitterPanes.h"

void CRightPane::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(6);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetExplorerApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 
}