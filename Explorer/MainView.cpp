/////////////////////////////////////////////
// MainView.cpp
//  Definitions for the CMainView class

#include "MainView.h"


CMainView::CMainView() : m_hwndOldFocus(NULL)
{
	SetPanes(GetTreeView(), GetListView());
	SetBarPos(200);
	SetImmediate(true);
}

LRESULT CMainView::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
{
    switch(((LPNMHDR)lParam)->code)
	{
 		// Store the control which just got focus
		case NM_SETFOCUS:
		{
			m_hwndOldFocus = ((LPNMHDR) lParam)->hwndFrom;
			return 0L;
		}

	} // switch LPNMHDR

	return 0L;
}

LRESULT CMainView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass any unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);	
}

