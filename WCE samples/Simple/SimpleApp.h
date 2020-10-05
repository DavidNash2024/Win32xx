/////////////////////////////
// SimpleApp.cpp


#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "wxx_wincore.h"
#include "view.h"


////////////////////////////////////////////////////////////
// CSimpleApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main dialog when it runs.
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
inline CSimpleApp* GetSimpleApp() { return static_cast<CSimpleApp*>(GetApp()); }


#endif //SIMPLEAPP_H
