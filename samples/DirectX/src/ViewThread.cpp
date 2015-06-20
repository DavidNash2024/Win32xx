//////////////////////////////////////////////
// ViewThread.cpp
//


#include "stdafx.h"
#include "DXApp.h"
#include "MainFrm.h"
#include "View.h"
#include "ViewThread.h"
#include "resource.h"

BOOL CViewThread::InitInstance()
{
	// This function runs when the thread starts

	// Create the view window
	//  Note: pFrame->GetDockClient() is the parent of the view window
	CMainFrame& Frame = GetDXApp().GetMainFrame();
	if (m_View.CreateEx(0, 0, 0, WS_CHILD, CRect(0,0,1,1), Frame.GetDockClient(), 0))
	{
		Frame.PostMessage(UWM_VIEWCREATED, 0, 0);
	}
	else
	{
		TRACE("\n*** ERROR *** Failed to create view window!\n\n");
		
		// Allow the main thread to resume
		SetEvent(GetView().GetCreateEvent());
	}

	return TRUE;	// return TRUE to run the message loop
}

int CViewThread::MessageLoop()
// Here we override CWinThread::MessageLoop to accommodate the special needs of DirectX
{
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));
	while( Msg.message!=WM_QUIT )
	{
		if( PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
		else
		{
			// Force thread to yield (otherwise it runs in a tight loop)
			if ( WAIT_TIMEOUT == WaitForSingleObject(GetThread(), 1) )
				m_View.Render();
		}
	}
	return LOWORD(Msg.wParam);
} 