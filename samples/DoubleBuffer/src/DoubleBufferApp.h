/////////////////////////////
// DoubleBufferApp.h
//

#ifndef DOUBLEBUFFERAPP_H
#define DOUBLEBUFFERAPP_H

#include "Mainfrm.h"

////////////////////////////////////////////
// Declaration of the CDoubleBufferApp class
//
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

#endif // define DOUBLEBUFFERAPP_H
