//////////////////////////////////////////////////
// FormApp.h

#pragma once

#include "Mainfrm.h"

///////////////////////////////////////////////////////////////
// CFormApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CFormApp : public CWinApp
{
public:
    CFormApp() = default;
    virtual ~CFormApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CFormApp(const CFormApp&) = delete;
    CFormApp& operator=(const CFormApp&) = delete;

    CMainFrame m_frame;
};

