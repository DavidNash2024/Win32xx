/////////////////////////////////////////////
// MainView.cpp
//  Definitions for the CMainView class

#include "MainView.h"


CMainView::CMainView(int nBarpos, bool bVertical) :
	CSplitter(nBarpos, bVertical)
{
	SetPanes(GetTreeView(), GetListView());
}

LRESULT CMainView::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	} 

	//Use the CSpliter default message handling for remaining messages
	return CSplitter::WndProc(hwnd, uMsg, wParam, lParam);
}

