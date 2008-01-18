#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "..\Win32++\Wincore.h"
#include "MainFrm.h"


class CFrameApp : public CWinApp
{
public:
    CFrameApp(HINSTANCE hInstance);
    virtual ~CFrameApp() {}
	virtual BOOL InitInstance();

private:
    CMainFrame m_Frame;
};




#endif //SIMPLEAPP_H

