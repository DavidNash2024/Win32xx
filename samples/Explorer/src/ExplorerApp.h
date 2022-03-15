//////////////////////////////////////////////////
// ExplorerApp.h
//

#ifndef EXPLORERAPP_H
#define EXPLORERAPP_H

#include "Mainfrm.h"

///////////////////////////////////////////////////////////////////
// CExplorerApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CExplorerApp : public CWinApp
{
public:
    CExplorerApp();
    virtual ~CExplorerApp();
    CMainFrame& GetMainFrame() { return m_frame; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};

// returns a pointer to the CExplorerApp object
inline CExplorerApp* GetExplorerApp() { return static_cast<CExplorerApp*>(GetApp()); }


#endif  //EXPLORERAPP_H
