/////////////////////////////
// Splash.cpp
//

#include "pch.h"
#include "SplashThread.h"


//////////////////////////////////////
// CSplashThread function definitions.
//

// Destructor.
CSplashThread::~CSplashThread()
{
    // Issue a close request to the splash window.
    if (m_splash.IsWindow())
        m_splash.Close();

    // End the thread.
    PostThreadMessage(WM_QUIT, 0, 0);
    if (::WaitForSingleObject(*this, 1000) == WAIT_TIMEOUT)
        Trace("Splash Thread failed to end cleanly\n");
}

// Called when the thread is started by CreateThread.
BOOL CSplashThread::InitInstance()
{
    m_splash.Create();
    m_splashCreated.SetEvent();
    return TRUE;
}