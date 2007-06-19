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

//	} 

	//Use the CSpliter default message handling for remaining messages
	return CSplitter::WndProc(hWnd, uMsg, wParam, lParam);
}

