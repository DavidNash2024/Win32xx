//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CViewMax and CMDIChildMax classes

#include "stdafx.h"
#include "MDIChildMax.h"
#include "resource.h"


// CViewMax definitions
void CViewMax::OnDraw(CDC* pDC)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	pDC->DrawText(_T("Maxed Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}


// CMDIChildMax definitions
CMDIChildMax::CMDIChildMax()
{
	HINSTANCE hResource = GetApp()->GetResourceHandle();
	HMENU hChildMenu = LoadMenu(hResource, _T("MdiMenuMax"));
	SetHandles(hChildMenu, NULL);
	SetView(m_MaxView);
}

CMDIChildMax::~CMDIChildMax()
{
}

void CMDIChildMax::OnInitialUpdate()
{
	SetWindowText(_T("Maximised Window"));
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
			Invalidate();
			return 0L;  // Continue with default processing

	}
	return WndProcDefault(uMsg, wParam, lParam);
}
