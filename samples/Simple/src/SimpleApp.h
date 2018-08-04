/////////////////////////////////////////
// SimpleApp.h

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


// Declaration of the CSimpleApp class
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp() {}
    virtual BOOL InitInstance();
    CView& GetView() { return m_view; }

private:
    CView m_view;
};


// returns a pointer to the CSimpleApp object
inline CSimpleApp& GetSimpleApp() { return static_cast<CSimpleApp&>(GetApp()); }


#endif
