/////////////////////////////
// App.h
//

#ifndef _APP_H_
#define _APP_H_

#include "wxx_wincore.h"
#include "MiniFrame.h"


////////////////////////////////////////////////////////////
// CApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main
// window when it runs.
class CApp : public CWinApp
{
public:
    CApp();
    virtual ~CApp() {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CApp(const CApp&) = delete;                // Disable copy construction
    CApp& operator = (const CApp&) = delete;   // Disable assignment operator

    CMiniFrame m_view;
};


#endif  // _APP_H_
