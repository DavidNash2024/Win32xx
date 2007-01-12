////////////////////////////////////////////////////
// Mainfrm.cpp  


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
	SetView(m_View);
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnInitialUpdate()
{
	//The frame is now created. 
	//Place any additional startup code here.

	//Disable some of the toolbar buttons
	GetToolbar().DisableButton(IDM_EDIT_CUT);
	GetToolbar().DisableButton(IDM_EDIT_COPY);
	GetToolbar().DisableButton(IDM_EDIT_PASTE);

	TRACE("Frame created");
}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here	
//	} 

	//Use the frame default message handling for remaining messages
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
}

