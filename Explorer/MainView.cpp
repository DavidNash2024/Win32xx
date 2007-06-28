/////////////////////////////////////////////
// MainView.cpp
//  Definitions for the CMainView class

#include "MainView.h"


CMainView::CMainView(int nBarpos, bool bVertical) :
	CSplitter(nBarpos, bVertical)
{
	SetPanes(GetTreeView(), GetListView());
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

