///////////////////////////////////////////////////////
//ThreadApp.cpp -  Definitions for the CThreadApp class

#include "stdafx.h"
#include "ThreadApp.h"


CThreadApp::CThreadApp()
{
}

CThreadApp::~CThreadApp()
{
}

BOOL CThreadApp::InitInstance()
{
	m_MainWnd.Create();	// throws a CWinException on failure  

	return TRUE;
}
