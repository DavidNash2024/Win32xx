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
    CSimpleRibbonApp() = default;
    virtual ~CSimpleRibbonApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CSimpleRibbonApp(const CSimpleRibbonApp&) = delete;
    CSimpleRibbonApp& operator=(const CSimpleRibbonApp&) = delete;

    CView m_view;
};


#endif
