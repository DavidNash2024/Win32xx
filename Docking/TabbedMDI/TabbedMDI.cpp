//////////////////////////////////////////////////////////////
// TabbedMDI.cpp -  Definitions for the CTabbedMDI, 
//                and CDockTabbedMDI classes


#include "TabbedMDIApp.h"
#include "TabbedMDI.h"
#include "resource.h"



///////////////////////////////////////////////
// CTabbedMDI functions
CWnd* CTabbedMDI::AddMDIChild(CWnd* pWnd, LPCTSTR szTabText)
{
	if (NULL == pWnd)
		throw CWinException(_T("Cannot add Null MDI Child"));

	// Set the extended style for a MDI child ????
//	CREATESTRUCT cs = {0};
//	cs.dwExStyle = WS_EX_MDICHILD;
//	pWnd->PreCreate(cs);
	
	m_Tab.AddTabPage(pWnd, szTabText);
	return pWnd;
}

HWND CTabbedMDI::Create(HWND hWndParent /* = NULL*/)
{
	CLIENTCREATESTRUCT clientcreate ;
	clientcreate.hWindowMenu  = m_hWnd;
	clientcreate.idFirstChild = IDW_FIRSTCHILD ;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | MDIS_ALLCHILDSTYLES;

	// Create the view window
	if (!CreateEx(0, _T("MDICLient"), _T(""),
		dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
			throw CWinException(_T("CMDIClient::Create ... CreateEx failed"));

	return m_hWnd;
}

LRESULT CTabbedMDI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_WINDOWPOSCHANGED:
	//	Invalidate();
		break;  
	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////
//  Definitions for the CDockTabbedMDI class
CDockTabbedMDI::CDockTabbedMDI()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockTabbedMDI::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	// Set the color of the splitter bar
	CMainFrame& MainFrame = GetTabbedMDIApp().GetMainFrame();
	CRebar& RB = MainFrame.GetRebar();
	if (RB.GetRebarTheme().UseThemes)
		SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	else
		SetBarColor(GetSysColor(COLOR_BTNFACE)); 

	SetCaption (_T("Simple View - Docking"));
}


