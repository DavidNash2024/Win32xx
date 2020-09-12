/////////////////////////////
// FrameApp.h
//

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"

/////////////////////////////////////
// Declaration of the CFrameApp class
//
class CFrameApp : public CWinApp
{
public:
    CFrameApp();
    virtual ~CFrameApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_Frame;
};


#endif // define FRAMEAPP_H
