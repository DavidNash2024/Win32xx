#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "wxx_wincore.h"
#include "view.h"


class CSimpleApp : public CWinApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp() {}
	virtual BOOL InitInstance();
	CView& GetView() { return m_View; }

private:
    CView m_View;
};


// returns a reference to the CSimpleApp object
inline CSimpleApp& GetSimpleApp() { return static_cast<CSimpleApp&>(GetApp()); }


#endif //SIMPLEAPP_H
