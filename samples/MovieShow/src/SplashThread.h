/////////////////////////////
// SplashThread.h
//

#ifndef SPLASHTHREAD_H
#define SPLASHTHREAD_H

#include "Splash.h"


//////////////////////////////////////////////////////////////////
// CSplashThread creates the windows thread for the splash window.
// The splash window is destroyed and the thread ends when the
// CSplashThread object goes out of scope.
class CSplashThread : public CWinThread
{
public:
    CSplashThread();
    virtual ~CSplashThread();
    CSplash* GetSplash() { return &m_splash; }
    CEvent& GetSplashCreated() { return m_splashCreated; }

protected:
    virtual BOOL InitInstance();

private:
    CSplashThread(const CSplashThread&);                // Disable copy construction
    CSplashThread& operator = (const CSplashThread&);   // Disable assignment operator

    CSplash m_splash;
    CEvent  m_splashCreated;
};


#endif // SPLASHTHREAD_H
