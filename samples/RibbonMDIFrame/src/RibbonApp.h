/////////////////////////////
// RibbonApp.h
//

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "MainMDIfrm.h"


/////////////////////////////////////////////////////////////////
// CRibbonFrameApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// MDI frame window when it runs.
class CRibbonFrameApp : public CWinApp
{
public:
    CRibbonFrameApp() = default;
    virtual ~CRibbonFrameApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CRibbonFrameApp(const CRibbonFrameApp&) = delete;
    CRibbonFrameApp& operator=(const CRibbonFrameApp&) = delete;

    CMainMDIFrame m_Frame;
};


#endif
