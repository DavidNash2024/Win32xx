/////////////////////////////
// FrameApp.h


#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "wxx_wincore.h"
#include "MainFrm.h"


////////////////////////////////////////////////////////////
// CWceFrameApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main dialog when it runs.
class CWceFrameApp : public CWinApp
{
public:
    CWceFrameApp();
    virtual ~CWceFrameApp() {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CWceFrameApp object
inline CWceFrameApp* GetFrameApp() { return static_cast<CWceFrameApp*>(GetApp()); }


#endif //SIMPLEAPP_H

