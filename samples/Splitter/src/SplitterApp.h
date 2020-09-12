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

private:
    CMainFrame m_frame;
};


#endif // define EFRAMEAPP_H
