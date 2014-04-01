/////////////////////////////////////////
// RibbonApp.h

#ifndef RIBBONAPP_H
#define RIBBONAPP_H

#include "MainFrm.h"


// Declaration of the CRibbonFrameApp class
class CRibbonFrameApp : public CWinApp
{
public:
    CRibbonFrameApp();
    virtual ~CRibbonFrameApp();
	virtual BOOL InitInstance();
	CMainFrame* GetFrame() { return &m_Frame; }

private:
    CMainFrame m_Frame;
};


// returns a pointer to the CRibbonFrameApp object
inline CRibbonFrameApp* GetSimpleApp() { return static_cast<CRibbonFrameApp*>(GetApp()); }


#endif
