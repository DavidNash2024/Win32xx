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
    CTextApp();
    virtual ~CTextApp()  {}
    virtual BOOL InitInstance();
    virtual BOOL OnIdle(LONG count);

private:
    CTextApp(const CTextApp&);                // Disable copy construction
    CTextApp& operator = (const CTextApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // define TEXTAPP_H
