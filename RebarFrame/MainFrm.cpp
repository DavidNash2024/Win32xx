////////////////////////////////////////////////////
//Mainfrm.cpp
// Definitions for the CMainFrame class

#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame() : m_hBitmap(NULL)
{
	SetView(m_View);
}

CMainFrame::~CMainFrame()
{
	if (m_hBitmap != NULL)
		::DeleteObject(m_hBitmap);
}

void CMainFrame::OnInitialUpdate()
{
	//Set Background
	m_hBitmap = ::LoadBitmap(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDB_BACKGROUND));
	SetBackground(m_hBitmap);
}


LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//	}

	//Use the frame default message handling for remaining messages
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
}


