/////////////////////////////////////////
// TabDemoApp.h

#ifndef TABDEMOAPP_H
#define TABDEMOAPP_H

#include "View.h"


// Declaration of the CTabDemoApp class
class CTabDemoApp : public CWinApp
{
public:
    CTabDemoApp(HINSTANCE hInstance);
    virtual ~CTabDemoApp() {}

private:
    CView m_View;
};

#endif
