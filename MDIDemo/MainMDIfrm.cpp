////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class


#include "resource.h"
#include "mainMDIfrm.h"
#include "MDIChildView.h"
#include "MDIChildRect.h"
#include "MDIChildMax.h"


CMainMDIFrame::CMainMDIFrame()
{
}

CMainMDIFrame::~CMainMDIFrame()
{
}

void CMainMDIFrame::OnInitialUpdate()
{
	//The frame is now created.
	//Place any additional startup code here.
}

BOOL CMainMDIFrame::OnCommand(UINT nID)
{
	switch (nID)
	{
	case IDM_FILE_NEWVIEW:
		AddMDIChild(new CMDIChildView);
		return 0;

	case IDM_FILE_NEWRECT:
		AddMDIChild(new CMDIChildRect);
		return 0;

	case IDM_FILE_NEWMAX:
		AddMDIChild(new CMDIChildMax);
		return 0;
	}
	return CMDIFrame::OnCommand(nID);
}

LRESULT CMainMDIFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	//Use the frame default message handling for remaining messages
	return CMDIFrame::WndProc(hwnd, uMsg, wParam, lParam);
}

