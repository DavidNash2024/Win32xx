////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame() : m_MainView(150, false)
{
	SetView(m_MainView);
}

CMainFrame::~CMainFrame()
{
}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		if (::IsWindow(m_MainView.GetHwnd()))
			m_MainView.Reposition();
		break;
	case USER_REARRANGED:
		if (::IsWindow(m_MainView.GetHwnd()))
			m_MainView.Reposition();
		break;
	}
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
}
