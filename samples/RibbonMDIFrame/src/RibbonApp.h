/////////////////////////////
// RibbonApp.h
//

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "MainMDIFrm.h"

////////////////////////////////////////////
// Declaration of the CRibbonFrameApp class.
//
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
