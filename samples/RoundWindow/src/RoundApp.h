/////////////////////////////
// RoundApp.h
//

#ifndef ROUNDAPP_H
#define ROUNDAPP_H

#include "View.h"


////////////////////////////////////////////////////////
// CRoundApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates
// the main window when it runs.
class CRoundApp : public CWinApp
{
public:
    CRoundApp() = default;
    virtual ~CRoundApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CRoundApp(const CRoundApp&) = delete;
    CRoundApp& operator=(const CRoundApp&) = delete;

    CView m_view;
};


#endif
