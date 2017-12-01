/////////////////////////////////////////
// RibbonApp.h

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "MainMDIFrm.h"


// Declaration of the CRibbonFrameApp class
class CRibbonFrameApp : public CWinApp
{
public:
    CRibbonFrameApp();
    virtual ~CRibbonFrameApp();
    virtual BOOL InitInstance();
    CMainMDIFrame& GetMainFrame() { return m_Frame; }

private:
    CMainMDIFrame m_Frame;
};


// returns a reference to the CRibbonFrameApp object
inline CRibbonFrameApp& GetRibbonFrameApp() { return static_cast<CRibbonFrameApp&>(GetApp()); }


#endif
