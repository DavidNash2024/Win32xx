/////////////////////////////
// DXApp.h
//

#ifndef DXAPP_H
#define DXAPP_H

#include "DXView.h"


/////////////////////////////////////////////////////////////
// CDXApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main
// window when it runs.
class CDXApp : public CWinApp
{
public:
    virtual ~CDXApp() {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();
    virtual int MessageLoop();

private:
    CDXView m_dxView;
};

#endif // DXAPP_H
