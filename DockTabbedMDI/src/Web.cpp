//////////////////////////////////////////////////////
// Web.cpp -  Definitions for the CViewWeb class

#include "stdafx.h"
#include "Web.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewWeb functions
LRESULT CViewWeb::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch(uMsg)
//	{
//	}

	return WndProcDefault(uMsg, wParam, lParam);
}

//////////////////////////////////////////////
//  Definitions for the CContainWeb class
CContainWeb::CContainWeb()
{
	SetView(m_ViewWeb);
	SetDockCaption (_T("Web View - Docking container"));
	SetTabText(_T("Web"));
	SetTabIcon(IDI_TEXT);
}


//////////////////////////////////////////////
//  Definitions for the CDockWeb class
CDockWeb::CDockWeb()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockWeb::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}

