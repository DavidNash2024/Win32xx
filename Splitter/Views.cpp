/////////////////////////////////////////////////////////
// Views.cpp -  Definitions for the CMainPane, CBottomPane,
//              and CMainView and CView classes


#include "SplitterApp.h"
#include "Views.h"



void CTopLeftView::OnPaint(HDC hDC)
{
	//Centre some text in the window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, _T("Top Left"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CTopRightView::OnPaint(HDC hDC)
{
	//Centre some text in the window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, _T("Top Right"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}


