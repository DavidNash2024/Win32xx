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

private:
    CMainFrame m_frame;
};


#endif // define TEXTAPP_H
