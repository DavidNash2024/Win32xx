/////////////////////////////
// SimpleApp.h
//

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"

///////////////////////////////////////
// Declaration of the CSimpleApp class.
//
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp() {}
    virtual BOOL InitInstance();

private:
    CView m_view;
};


#endif
