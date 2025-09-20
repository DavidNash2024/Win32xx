/////////////////////////////
// SplitterApp.h
//

#ifndef SPLITTERAPP_H
#define SPLITTERAPP_H

#include "Mainfrm.h"


///////////////////////////////////////////////////////////////////
// CSplitterApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CSplitterApp : public CWinApp
{
public:
    CSplitterApp() = default;
    virtual ~CSplitterApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CSplitterApp(const CSplitterApp&) = delete;
    CSplitterApp& operator=(const CSplitterApp&) = delete;

    CMainFrame m_frame;
};


#endif // define EFRAMEAPP_H
