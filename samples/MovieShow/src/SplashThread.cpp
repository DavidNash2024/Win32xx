/////////////////////////////
// Splash.cpp
//

#include "stdafx.h"
#include "SplashThread.h"


///////////////////////////////
// CSplashThread function definitions
//

// Constructor.
CSplashThread::CSplashThread()
{
}

// Destructor.
CSplashThread::~CSplashThread()
{
    // Destroy the thread's window.
    m_splash.Destroy();

    // End the thread.
    PostThreadMessage(WM_QUIT, 0, 0);
    if (::WaitForSingleObject(*this, 1000) == WAIT_TIMEOUT)
        Trace("Splash Thread failed to end cleanly\n");
}

// Called when the thread is started by CreateThread.
BOOL CSplashThread::InitInstance()
{
    m_splash.Create();
    return TRUE;
}