/////////////////////////////
// TextApp.h
//

#ifndef TEXTAPP_H
#define TEXTAPP_H

#include "Mainfrm.h"

////////////////////////////////////
// Declaration of the CTextApp class
//
class CTextApp : public CWinApp
{
public:
    CTextApp();
    virtual ~CTextApp()  {}
    virtual BOOL InitInstance();
    virtual BOOL OnIdle(LONG count);
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CTextApp object
inline CTextApp* GetTextApp() { return static_cast<CTextApp*>(GetApp()); }


#endif // define TEXTAPP_H

