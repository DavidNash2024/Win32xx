/////////////////////////////
// FrameApp.h
//

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"

////////////////////////////////////////////
// Declaration of the CToolBarDemoApp class.
//
class CToolBarDemoApp : public CWinApp
{
public:
    CToolBarDemoApp();
    virtual ~CToolBarDemoApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define FRAMEAPP_H
