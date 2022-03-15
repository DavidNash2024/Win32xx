/////////////////////////////
// DoubleBufferApp.h
//

#ifndef DOUBLEBUFFERAPP_H
#define DOUBLEBUFFERAPP_H

#include "Mainfrm.h"


///////////////////////////////////////////////////////////////
// CDoubleBufferApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main window when it runs.
class CDoubleBufferApp : public CWinApp
{
public:
    CDoubleBufferApp();
    virtual ~CDoubleBufferApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};

#endif // define DOUBLEBUFFERAPP_H
