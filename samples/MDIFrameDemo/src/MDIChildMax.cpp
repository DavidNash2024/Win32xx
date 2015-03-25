//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CViewMax and CMDIChildMax classes

#include "stdafx.h"
#include "MDIChildMax.h"
#include "resource.h"


// CViewMax definitions
void CViewMax::OnDraw(CDC& dc)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	dc.DrawText(_T("Maxed Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewMax::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
			Invalidate();
			return 0L;

	}
	return WndProcDefault(uMsg, wParam, lParam);
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

int CMDIChildMax::OnCreate(LPCREATESTRUCT pcs)
{
	SetWindowText(_T("Maximised Window"));
	SetIconLarge(IDI_MAX);
	SetIconSmall(IDI_MAX);

	return CMDIChild::OnCreate(pcs);
}

void CMDIChildMax::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_MAXIMIZE;
}

