/////////////////////////////////////////////
// MainView.cpp
//  Definitions for the CMainView class

#include "MainView.h"


CMainView::CMainView(int nBarpos, bool bVertical) :
	CSplitter(nBarpos, bVertical)
{
	SetPanes(GetTreeView(), GetListView());
}



