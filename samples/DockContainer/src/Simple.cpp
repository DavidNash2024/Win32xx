//////////////////////////////////////////////////////////////
// Simple.cpp -  Definitions for the CViewSimple, 
//                and CDockSimple classes

#include "stdafx.h"
#include "ContainerApp.h"
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

//////////////////////////////////////////////
//  Definitions for the CDockSimple class
CDockSimple::CDockSimple()
{
	// Set the view window to our edit control
	SetView(m_View);
}

CDocker* CDockSimple::NewDockerFromID(int nID)
{
	CDocker* pDock = NULL;
	switch(nID)
	{
	case ID_DOCK_CLASSES1:
		pDock = new CDockClasses;
		break;
	case ID_DOCK_CLASSES2:
		pDock = new CDockClasses;					
		break;
	case ID_DOCK_FILES1:
		pDock = new CDockFiles;
		break;
	case ID_DOCK_FILES2:
		pDock = new CDockFiles;
		break;
	case ID_DOCK_OUTPUT1:
		pDock = new CDockOutput;
		break;
	case ID_DOCK_OUTPUT2:
		pDock = new CDockOutput;
		break;
	case ID_DOCK_TEXT1:
		pDock = new CDockText;
		break;
	case ID_DOCK_TEXT2:
		pDock = new CDockText;
		break;
	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDock;
}

int CDockSimple::OnCreate(LPCREATESTRUCT pcs)
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	return CDocker::OnCreate(pcs);
}


