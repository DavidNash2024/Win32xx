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
    CExplorerApp() = default;
    virtual ~CExplorerApp() override;
    CMainFrame& GetMainFrame() { return m_frame; }

protected:
    virtual BOOL InitInstance() override;

private:
    CExplorerApp(const CExplorerApp&) = delete;
    CExplorerApp& operator=(const CExplorerApp&) = delete;

    CMainFrame m_frame;
};

// returns a pointer to the CExplorerApp object
inline CExplorerApp* GetExplorerApp() { return static_cast<CExplorerApp*>(GetApp()); }


#endif  //EXPLORERAPP_H
