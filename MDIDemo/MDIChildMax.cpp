//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CMDIChildMax class

#include "MDIChildMax.h"
#include "resource.h"


CMDIChildMax::CMDIChildMax()
{
	SetChildMenu(_T("MdiMenuMax"));
}

CMDIChildMax::~CMDIChildMax()
{
}

void CMDIChildMax::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("Maximised Window"));
	SetIconLarge(IDI_MAX);
	SetIconSmall(IDI_MAX);
}

void CMDIChildMax::OnPaint(HDC hDC)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	::DrawText(hDC, _T("Maxed Window"), -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CMDIChildMax::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_MAXIMIZE;
}

LRESULT CMDIChildMax::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
			::InvalidateRect(hWnd, NULL, TRUE);
			break;  // Continue with default processing

	}
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}
