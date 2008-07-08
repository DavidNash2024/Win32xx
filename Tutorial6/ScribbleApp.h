//////////////////////////////////
// ScribbleApp.h


#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "../DevWin++/Wincore.h"
#include "Mainfrm.h"


class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
	virtual BOOL InitInstance();

private:
    CMainFrame m_Frame;
};

#endif //SCRIBBLEAPP_H

