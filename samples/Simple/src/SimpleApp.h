/////////////////////////////
// SimpleApp.h
//

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


////////////////////////////////////////////////////////////
// CSimpleApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main window when it runs.
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp() {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CSimpleApp(const CSimpleApp&);                // Disable copy construction
    CSimpleApp& operator = (const CSimpleApp&);   // Disable assignment operator

    CView m_view;
};


#endif
