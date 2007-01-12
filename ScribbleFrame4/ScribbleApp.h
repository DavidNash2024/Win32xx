////////////////////////////////////////
// ScribbleApp.h


#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "..\Win32++\Wincore.h"
#include "Mainfrm.h"



class CScribbleApp : public CWinApp
{
public:
    CScribbleApp(HINSTANCE hInstance);
    virtual ~CScribbleApp() {}

private:
    CMainFrame m_Frame;
};

#endif //SCRIBBLEAPP_H

