//////////////////////////////////////////////////////////////
// Simple.cpp -  Definitions for the CViewSimple, 
//                and CDockSimple classes


#include "ContainerApp.h"
#include "Simple.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewSimple functions
void CViewSimple::OnPaint(HDC hDC)
{
	//Centre some text in the window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, _T("Simple View"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SIZE:
		Invalidate();
		break;
	
	case WM_WINDOWPOSCHANGED:
		Invalidate();
		break;
	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////
//  Definitions for the CDockSimple class
CDockSimple::CDockSimple()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockSimple::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}


