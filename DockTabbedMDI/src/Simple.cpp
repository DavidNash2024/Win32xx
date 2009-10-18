//////////////////////////////////////////////////////
// Simple.cpp -  Definitions for the CViewSimple class


#include "stdafx.h"
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

LRESULT CViewSimple::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	return WndProcDefault(uMsg, wParam, lParam);
}

