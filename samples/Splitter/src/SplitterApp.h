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
    CSplitterApp();
    virtual ~CSplitterApp()  {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CSplitterApp(const CSplitterApp&);                // Disable copy construction
    CSplitterApp& operator = (const CSplitterApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // define EFRAMEAPP_H
