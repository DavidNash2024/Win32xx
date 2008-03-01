#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "..\Win32++\Wincore.h"
#include "MainFrm.h"


class CWceFrameApp : public CWinApp
{
public:
    CWceFrameApp();
    virtual ~CWceFrameApp() {}
	virtual BOOL InitInstance();

private:
    CMainFrame m_Frame;
};



#endif //SIMPLEAPP_H

