/////////////////////////////
// SimpleApp.h
//

#pragma once

#include "View.h"

////////////////////////////////////////////////////////////
// CSimpleApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main window when it runs.
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp() = default;
    virtual ~CSimpleApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CSimpleApp(const CSimpleApp&) = delete;
    CSimpleApp& operator=(const CSimpleApp&) = delete;

    CView m_view;
};

