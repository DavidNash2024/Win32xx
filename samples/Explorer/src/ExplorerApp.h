//////////////////////////////////////////////////
// ExplorerApp.h
//  Declaration of the CExplorerApp class

#ifndef EXPLORERAPP_H
#define EXPLORERAPP_H


#include "Mainfrm.h"


class CExplorerApp : public CWinApp
{
public:
    CExplorerApp();
    virtual ~CExplorerApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_Frame; }

private:
    CMainFrame m_Frame;
};

// returns a reference to the CExplorerApp object
inline CExplorerApp& GetExplorerApp() { return static_cast<CExplorerApp&>(GetApp()); }


#endif  //EXPLORERAPP_H

