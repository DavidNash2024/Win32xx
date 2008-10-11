//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CMaxView and CMaxMDIChild classes

#include "MaxMDIChild.h"
#include "resource.h"


// CMaxView definitions
void CMaxView::OnPaint(HDC hDC)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	::DrawText(hDC, _T("Maxed Window"), -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}


// CMaxMDIChild definitions
CMaxMDIChild::CMaxMDIChild()
{
	SetChildMenu(_T("MdiMenuMax"));
	SetView(m_MaxView);
}

CMaxMDIChild::~CMaxMDIChild()
{
}

void CMaxMDIChild::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("Maximised Window"));
	SetIconLarge(IDI_MAX);
	SetIconSmall(IDI_MAX);
}

void CMaxMDIChild::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_MAXIMIZE;
}

LRESULT CMaxMDIChild::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
			::InvalidateRect(hWnd, NULL, TRUE);
			break;  // Continue with default processing

	}
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}
