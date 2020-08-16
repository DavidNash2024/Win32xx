/////////////////////////////
// RibbonApp.h
//

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "MainFrm.h"

////////////////////////////////////////////
// Declaration of the CRibbonFrameApp class.
//
class CRibbonFrameApp : public CWinApp
{
public:
    CRibbonFrameApp();
    virtual ~CRibbonFrameApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CRibbonFrameApp object
inline CRibbonFrameApp* GetRibbonFrameApp() { return static_cast<CRibbonFrameApp*>(GetApp()); }


#endif
