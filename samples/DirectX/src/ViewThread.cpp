//////////////////////////////////////////////
// ViewThread.cpp
//


#include "stdafx.h"
#include "View.h"
#include "MainFrm.h"
#include "resource.h"
#include "ViewThread.h"

BOOL CViewThread::InitInstance()
{
	// This function runs when the thread starts

	// Create the view window
	m_View.CreateEx(0, 0, 0, WS_CHILD, CRect(50,50,400,400), m_pFrame, 0);

	m_pFrame->PostMessage(UWM_VIEWCREATED, 0, 0);

	return TRUE;	// return TRUE to run the message loop
}

int CViewThread::MessageLoop()
// Here we override CThread::MessageLoop to accommodate the special needs of DirectX
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
		{
			if ( WAIT_TIMEOUT == WaitForSingleObject(GetThread(), 1) )
				m_View.Render();
		}
	}
	return LOWORD(Msg.wParam);
} 