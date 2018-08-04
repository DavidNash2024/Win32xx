//////////////////////////////////////////////////
// DoubleBufferApp.h

#ifndef DOUBLEBUFFERAPP_H
#define DOUBLEBUFFERAPP_H

#include "Mainfrm.h"


// Declaration of the CDoubleBufferApp class
class CDoubleBufferApp : public CWinApp
{
public:
    CDoubleBufferApp();
    virtual ~CDoubleBufferApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// a useful function that returns a reference to the CDoubleBufferApp object
inline CDoubleBufferApp& GetFrameApp() { return static_cast<CDoubleBufferApp&>(GetApp()); }


#endif // define DOUBLEBUFFERAPP_H

