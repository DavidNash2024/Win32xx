//////////////////////////////////////////////////////
// Simple.cpp -  Definitions for the CViewSimple class

#include "stdafx.h"
#include "Simple.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewSimple functions
void CViewSimple::OnDraw(CDC* pDC)
{
	//Centre some text in the window
	CRect rc = GetClientRect();
	pDC->DrawText(_T("Simple View"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::OnSize(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Invalidate();
	return 0L;
}

LRESULT CViewSimple::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Invalidate();
	return 0L;
}

LRESULT CViewSimple::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SIZE:				return OnSize(wParam, lParam);
	case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
	}

	return WndProcDefault(uMsg, wParam, lParam);
}
