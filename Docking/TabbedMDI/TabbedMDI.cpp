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
	
	m_Tab.AddTabPage(pWnd, szTabText);
	if (!m_Tab.IsWindow())
	{
		m_Tab.Create(m_hWnd);
		RecalcLayout();
	}

	return pWnd;
}

HWND CTabbedMDI::Create(HWND hWndParent /* = NULL*/)
{
	CLIENTCREATESTRUCT clientcreate ;
	clientcreate.hWindowMenu  = m_hWnd;
	clientcreate.idFirstChild = IDW_FIRSTCHILD ;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | MDIS_ALLCHILDSTYLES;

	// Create the view window
	if (!CreateEx(0, _T("MDICLient"), _T(""),
		dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
			throw CWinException(_T("CMDIClient::Create ... CreateEx failed"));

	return m_hWnd;
}

void CTabbedMDI::RecalcLayout()
{
	if (m_Tab.GetItemCount() >0)
	{
		CRect rc = GetClientRect();
		m_Tab.SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}
	else
		m_Tab.ShowWindow(SW_HIDE);
}

LRESULT CTabbedMDI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		if (m_Tab.GetItemCount() >0)
			return 0;
		break;
	case WM_WINDOWPOSCHANGED:
		RecalcLayout();
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


