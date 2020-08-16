/////////////////////////////
// SplitterApp.h
//

#ifndef SPLITTERAPP_H
#define SPLITTERAPP_H

#include "Mainfrm.h"

/////////////////////////////////////////
// Declaration of the CSplitterApp class.
//
class CSplitterApp : public CWinApp
{
public:
    CSplitterApp();
    virtual ~CSplitterApp()  {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};

// returns a pointer to the CSplitterApp object
inline CSplitterApp* GetSplitApp() { return static_cast<CSplitterApp*>(GetApp()); }

#endif // define EFRAMEAPP_H
