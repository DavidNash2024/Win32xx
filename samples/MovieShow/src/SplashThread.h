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
    virtual ~CSplashThread() override;
    CSplashThread(const CSplashThread&) = delete;
    CSplashThread& operator=(const CSplashThread&) = delete;

    CSplash* GetSplash() { return &m_splash; }
    CEvent& GetSplashCreated() { return m_splashCreated; }

protected:
    virtual BOOL InitInstance() override;

private:
    CSplash m_splash;
    CEvent  m_splashCreated;
};


#endif // SPLASHTHREAD_H
