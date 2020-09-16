/////////////////////////////
// SimpleApp.h
//

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


/////////////////////////////////////////////////////////////////
// CSimpleRibbonApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// window when it runs.
class CSimpleRibbonApp : public CWinApp
{
public:
    CSimpleRibbonApp();
    virtual ~CSimpleRibbonApp();
    virtual BOOL InitInstance();

private:
    CView m_view;
};


#endif
