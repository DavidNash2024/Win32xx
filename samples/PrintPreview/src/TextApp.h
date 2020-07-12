//////////////////////////////////////////////////
// TextApp.h
//  Declaration of the CTextApp class

#ifndef TEXTAPP_H
#define TEXTAPP_H

#include "Mainfrm.h"


class CTextApp : public CWinApp
{
public:
    CTextApp(); 
    virtual ~CTextApp()  {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }
    BOOL OnIdle(LONG count);

private:
    CMainFrame m_frame;
};


// returns a pointer to the CTextApp object
inline CTextApp* GetTextApp() { return static_cast<CTextApp*>(GetApp()); }


#endif // define TEXTAPP_H

