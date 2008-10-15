/////////////////////////////////////////////////////////
// Views.cpp -  Definitions for the CMainPane, CBottomPane,
//              and CMainView and CView classes


#include "SplitterApp.h"
#include "Views.h"



void CMySimpleView::OnPaint(HDC hDC)
{
	//Centre some text in the window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, _T("Simple View"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}



