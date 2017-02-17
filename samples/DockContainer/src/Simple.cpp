//////////////////////////////////////////////////////////////
// Simple.cpp -  Definitions for the CViewSimple, 
//                and CDockSimple classes

#include "stdafx.h"
#include "Simple.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewSimple functions
void CViewSimple::OnDraw(CDC& dc)
{
	//Centre some text in the window
	CRect rc = GetClientRect();
	dc.DrawText(_T("Simple View"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CViewSimple::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Invalidate();
	return 0L;
}

LRESULT CViewSimple::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEACTIVATE:		return OnMouseActivate(uMsg, wParam, lParam);
	case WM_SIZE:				return OnSize(uMsg, wParam, lParam);
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

