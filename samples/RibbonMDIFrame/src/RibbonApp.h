/////////////////////////////
// RibbonApp.h
//

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "MainMDIFrm.h"


/////////////////////////////////////////////////////////////////
// CRibbonFrameApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// MDI frame window when it runs.
class CRibbonFrameApp : public CWinApp
{
public:
    CRibbonFrameApp();
    virtual ~CRibbonFrameApp();
    virtual BOOL InitInstance();

private:
    CMainMDIFrame m_Frame;
};


#endif
