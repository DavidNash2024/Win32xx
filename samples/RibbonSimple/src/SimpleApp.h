/////////////////////////////
// SimpleApp.h
//

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"

/////////////////////////////////////////////
// Declaration of the CSimpleRibbonApp class.
//
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
