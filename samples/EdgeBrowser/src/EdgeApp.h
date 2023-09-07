/////////////////////////////
// CEdgeApp.h
//

#ifndef _EDGEAPP_H_
#define _EDGEAPP_H_

#include "EdgeView.h"


////////////////////////////////////////////////////////////
// CEdgeApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main window when it runs.
class CEdgeApp : public CWinApp
{
public:
    CEdgeApp();
    virtual ~CEdgeApp() {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CEdgeApp(const CEdgeApp&);                // Disable copy construction
    CEdgeApp& operator = (const CEdgeApp&);   // Disable assignment operator

    CEdgeView m_view;
};

#endif // _EDGEAPP_H_
