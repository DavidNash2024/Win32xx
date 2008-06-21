/////////////////////////////////////////////////////////
// Views.cpp -  Definitions for the CTopView, CBottomView,
//              and CMainView and CView classes


#include "SplitterApp.h"
#include "Views.h"


/////////////////////////////
// CView function definitions
CView::CView()
{
	m_str[0] = _T('\0');
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::OnPaint(HDC hDC)
{
	//Centre some text in the window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, m_str, -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::SetString(TCHAR* str)
{
	//sets the string centred in the window
	::lstrcpyn(m_str, str, 40);
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		//Repaint the window when it's resized
		::InvalidateRect(m_hWnd, NULL, TRUE);
		break;
	}
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

////////////////////////////////
// CTopView function definitions
CTopView::CTopView()
{
	SetBarPos(100);
	SetImmediate(true);
	SetVertical(true);
	m_TopLeft.SetString(_T("Top Left"));
	m_TopRight.SetString(_T("Top Right"));

	//Set the two client windows panes of top splitter window
	SetPanes(m_TopLeft, m_TopRight);
}

///////////////////////////////////
// CBottomView function definitions
CBottomView::CBottomView()
{
	SetBarPos(100);
	SetImmediate(true);
	SetVertical(true);
	m_BottomLeft.SetString(_T("Bottom Left"));
	m_BottomRight.SetString(_T("Bottom Right"));

	//Set the two client windows panes of the bottom splitter window
	SetPanes(m_BottomLeft, m_BottomRight);
}

/////////////////////////////////
// CMainView function definitions
CMainView::CMainView() 
{
	SetBarPos(150);
	SetImmediate(true);
	SetVertical(false);
	SetWidth(15);
	SetPanes(m_Top, m_Bottom);
}

void CMainView::Reposition()
//Repositions the splitter windows
{
	//A handy trick to get a pointer to our CMainFrame object
	CMainFrame& MainFrame = GetSplitApp().GetMainFrame();

	//Get the client area of our frame
	CRect rc = MainFrame.GetViewRect();

	int pos = (rc.Height())/2;
	SetBarPos(pos);
	pos = (rc.Width())/3;
	m_Top.SetBarPos(pos);
	m_Bottom.SetBarPos(pos);
}

LRESULT CMainView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Additional messages to be handled go here
//	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

