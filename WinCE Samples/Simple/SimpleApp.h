#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "..\Win32++\Wincore.h"
#include "view.h"


class CSimpleApp : public CWinApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp() {}
	virtual BOOL InitInstance();

private:
    CView m_View;
};




#endif //SIMPLEAPP_H
