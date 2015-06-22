//////////////////////////////////////////////
// MyTabbedMDI.cpp

#include "stdafx.h"
#include "MyTabbedMDI.h"
#include "Mainfrm.h"

CMyTabbedMDI::CMyTabbedMDI()
{
}

CWnd* CMyTabbedMDI::NewMDIChildFromID(int idMDIChild)
{
	CWnd* pView = NULL;
	switch(idMDIChild)
	{
	case ID_MDI_CLASSES:
		pView = new CViewClasses;
		break;
	case ID_MDI_FILES:
		pView = new CViewFiles;
		break;
	case ID_MDI_OUTPUT:
		pView = new CViewOutput;
		break;
	case ID_MDI_TEXT:
		pView = new CViewText;
		break;
	case ID_MDI_RECT:
		pView = new CViewRect;
		break;
	case ID_MDI_WEB:
		pView = new CViewWeb;
		break;
	default:
		TRACE("Unknown TabbedMDI id\n");
		break;
	}

	return pView;
}

LRESULT CMyTabbedMDI::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT_PTR nIDTimer = 101;

	switch(uMsg)
	{
	case WM_PARENTNOTIFY:
		{
			if (LOWORD(wParam) == WM_LBUTTONDOWN)
			{
				// Test if we are over a tab. If so, set at timer.
				CPoint pt = GetCursorPos();
				ScreenToClient(pt);
				TCHITTESTINFO info;
				ZeroMemory(&info, sizeof(TCHITTESTINFO));
				info.pt = pt;
				GetTab().HitTest(info);
				if (info.flags != TCHT_NOWHERE) 
					SetTimer(nIDTimer, 100, NULL);
			}
		}
		break;
	case WM_TIMER:
		if (wParam == nIDTimer)
		{
			KillTimer(nIDTimer);

			// return focus to active child when tab is pressed
			GetActiveMDIChild()->SetFocus();
		}
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

