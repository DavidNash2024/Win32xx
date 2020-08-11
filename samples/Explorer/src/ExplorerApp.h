//////////////////////////////////////////////////
// ExplorerApp.h
//

#ifndef EXPLORERAPP_H
#define EXPLORERAPP_H

#include "Mainfrm.h"

////////////////////////////////////////
// Declaration of the CExplorerApp class
//
class CExplorerApp : public CWinApp
{
public:
    CExplorerApp();
    virtual ~CExplorerApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};

// returns a pointer to the CExplorerApp object
inline CExplorerApp* GetExplorerApp() { return static_cast<CExplorerApp*>(GetApp()); }


#endif  //EXPLORERAPP_H
