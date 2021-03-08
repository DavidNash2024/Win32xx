/////////////////////////////
// DXApp.h
//

#ifndef DXAPP_H
#define DXAPP_H

#include "Mainfrm.h"


///////////////////////////////////////////////////////////////
// CDXApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CDXApp : public CWinApp
{
public:
    CDXApp();
    virtual ~CDXApp() {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CDXApp object
inline CDXApp* GetDXApp() { return static_cast<CDXApp*>(GetApp()); }

#endif
