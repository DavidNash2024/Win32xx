/////////////////////////////
// ScribbleApp.h
//

#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "Mainfrm.h"

/////////////////////////////////////////
// Declaration of the CScribbleApp class.
//
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif //SCRIBBLEAPP_H
