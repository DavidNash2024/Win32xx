//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CViewMax and CMDIChildMax classes

#include "MDIChildMax.h"
#include "resource.h"


// CViewMax definitions
void CViewMax::OnPaint(HDC hDC)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	::DrawText(hDC, _T("Maxed Window"), -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}


// CMDIChildMax definitions
CMDIChildMax::CMDIChildMax()
{
	SetChildMenu(_T("MdiMenuMax"));
	SetView(m_MaxView);
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

void CMDIChildMax::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_MAXIMIZE;
}

LRESULT CMDIChildMax::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;  // Continue with default processing

	}
	return WndProcDefault(uMsg, wParam, lParam);
}
