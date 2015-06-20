/////////////////////////////////////////
// SimpleApp.h

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


// Declaration of the CSimpleRibbonApp class
class CSimpleRibbonApp : public CWinApp
{
public:
    CSimpleRibbonApp();
    virtual ~CSimpleRibbonApp();
	virtual BOOL InitInstance();
	CView& GetView() { return m_View; }

private:
    CView m_View;
};


// returns a pointer to the CSimpleRibbonApp object
inline CSimpleRibbonApp& GetSimpleApp() { return static_cast<CSimpleRibbonApp&>(*GetApp()); }


#endif
