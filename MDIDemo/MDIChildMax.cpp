//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CMDIChildMax class

#include "MDIChildMax.h"
#include "resource.h"


CMDIChildMax::CMDIChildMax()
{
	SetChildMenu(TEXT ("MdiMenuMax"));
}

CMDIChildMax::~CMDIChildMax()
{
}

void CMDIChildMax::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, TEXT("Maximised Window"));
	SetIconLarge(IDI_MAX);
	SetIconSmall(IDI_MAX);
}

void CMDIChildMax::OnPaint(HDC hDC)
{
	//Centre some text in our view window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, TEXT("Maxed Window"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CMDIChildMax::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_MAXIMIZE;
}

LRESULT CMDIChildMax::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;  // Continue with default processing 

	}
	return CMDIChild::WndProc(hwnd, uMsg, wParam, lParam);
}
