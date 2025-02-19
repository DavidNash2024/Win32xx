/////////////////////////////
// TextApp.h
//

#ifndef TEXTAPP_H
#define TEXTAPP_H

#include "Mainfrm.h"


///////////////////////////////////////////////////////////////
// CTextApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CTextApp : public CWinApp
{
public:
    CTextApp() = default;
    virtual ~CTextApp() override = default;

protected:
    virtual BOOL InitInstance() override;
    virtual BOOL OnIdle(LONG count) override;

private:
    CTextApp(const CTextApp&) = delete;
    CTextApp& operator=(const CTextApp&) = delete;

    CMainFrame m_frame;
};


#endif // define TEXTAPP_H
