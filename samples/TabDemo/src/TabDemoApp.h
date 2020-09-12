/////////////////////////////
// TabDemoApp.h
//

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H


#include "Mainfrm.h"

////////////////////////////////////////
// Declaration of the CTabDemoApp class.
//
class CTabDemoApp : public CWinApp
{
public:
    CTabDemoApp();
    virtual ~CTabDemoApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // CONTAINERAPP_H
