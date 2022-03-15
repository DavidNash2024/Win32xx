/////////////////////////////
// App.h
//

#ifndef SDI_APP_H
#define SDI_APP_H

#include "Mainfrm.h"

///////////////////////////////////////////////////////////////
// CApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CApp : public CWinApp
{
public:
    CApp();
    virtual ~CApp();

protected:
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define SDI_APP_H
