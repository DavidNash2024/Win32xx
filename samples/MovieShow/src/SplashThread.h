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
    virtual BOOL InitInstance();

    CSplash* GetSplash() { return &m_splash; }

private:
    CSplash m_splash;
};


#endif // SPLASHTHREAD_H
