//////////////////////////////////////////////
// MyThread.cpp
//  Definitions for the CMyThread class

#include "stdafx.h"
#include "MyThread.h"
#include "Mainfrm.h"


CMyThread::CMyThread() : m_hwndParent(0)
{
}

void CMyThread::StartThread(HWND hwndParent)
{
	m_hwndParent = hwndParent;
	ResumeThread();
}

BOOL CMyThread::InitInstance()
{
	// This function runs when the thread starts

	// Create a test window for this thread
	m_View.Create(m_hwndParent);

	PostMessage(m_hwndParent, UWM_VIEWCREATED, 0, 0);

	return TRUE;	// return TRUE to run the message loop
}

int CMyThread::MessageLoop()
// Override CWinApp::MessageLoop to accommodate the needs of DirectX
{
	MSG Msg = {0};
	while( Msg.message!=WM_QUIT )
	{
		if( PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
		else
			m_View.Render();
	}
	return LOWORD(Msg.wParam);
} 

