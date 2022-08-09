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

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CSimpleRibbonApp(const CSimpleRibbonApp&);                // Disable copy construction
    CSimpleRibbonApp& operator = (const CSimpleRibbonApp&);   // Disable assignment operator

    CView m_view;
};


#endif
