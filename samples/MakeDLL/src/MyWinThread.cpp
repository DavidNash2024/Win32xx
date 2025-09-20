/////////////////////////////
// MyDWinThread.cpp
//

#include "StdAfx.h"
#include "MyDialog.h"
#include "MyWinThread.h"
#include "resource.h"


// Constructor.
CMyWinThread::CMyWinThread() : m_myDialog(IDD_DIALOG1)
{
}

// InitInstance is called when the thread is created.
// Return TRUE to run the message loop for the thread.
BOOL CMyWinThread::InitInstance()
{
    m_myDialog.Create();
    return TRUE;
}

