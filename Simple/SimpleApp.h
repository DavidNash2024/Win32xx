/////////////////////////////////////////
// SimpleApp.h

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


// Declaration of the CWinApp class
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp(HINSTANCE hInstance);
    virtual ~CSimpleApp() {}

private:
    CView m_View;
};

#endif
