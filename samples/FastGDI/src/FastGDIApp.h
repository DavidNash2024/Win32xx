//////////////////////////////////////////////////
// FastGDIApp.h

#ifndef FASTGDIAPP_H
#define FASTGDIAPP_H

#include "Mainfrm.h"

//////////////////////////////////////////////////////////////////
// CFastGDIApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CFastGDIApp : public CWinApp
{
public:
    CFastGDIApp() = default;
    virtual ~CFastGDIApp() override = default;
    CMainFrame& GetMainFrame() { return m_frame; }

protected:
    virtual BOOL InitInstance() override;
    virtual BOOL OnIdle(LONG) override;

private:
    CFastGDIApp(const CFastGDIApp&) = delete;
    CFastGDIApp& operator=(const CFastGDIApp&) = delete;

    CMainFrame m_frame;
};

#endif // define FASTGDIAPP_H
