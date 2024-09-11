/////////////////////////////
// RibbonApp.h

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////////////////////////
// CRibbonFrameApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// frame window when it runs.
class CRibbonFrameApp : public CWinApp
{
public:
    CRibbonFrameApp();
    virtual ~CRibbonFrameApp() override;

protected:
    virtual BOOL InitInstance() override;

private:
    CRibbonFrameApp(const CRibbonFrameApp&) = delete;
    CRibbonFrameApp& operator=(const CRibbonFrameApp&) = delete;

    CMainFrame m_frame;
};


#endif
