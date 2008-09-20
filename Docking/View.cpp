//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"


CView::CView()
{
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the Window Class name to an edit control
	cs.lpszClass = _T("Edit");
	cs.style = WS_VISIBLE | WS_CHILD | ES_MULTILINE;
}



