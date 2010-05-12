//////////////////////////////////////////////////////
// Web.cpp -  Definitions for the CViewBrowser class

#include "stdafx.h"
#include "Web.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewBrowser functions
LRESULT CViewBrowser::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch(uMsg)
//	{
//	}

	return WndProcDefault(uMsg, wParam, lParam);
}

//////////////////////////////////////////////
//  Definitions for the CContainBrowser class
CContainBrowser::CContainBrowser()
{
	SetView(m_ViewWeb);
	SetDockCaption (_T("Browser View - Docking container"));
	SetTabText(_T("Browser"));
	SetTabIcon(IDI_BROWSER);
}


//////////////////////////////////////////////
//  Definitions for the CDockBrowser class
CDockBrowser::CDockBrowser()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockBrowser::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}

