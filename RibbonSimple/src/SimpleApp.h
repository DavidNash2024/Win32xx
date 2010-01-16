/////////////////////////////////////////
// SimpleApp.h

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


// Declaration of the CWinApp class
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


// returns a reference to the CSimpleRibbonApp object
inline CSimpleRibbonApp& GetSimpleApp() { return *((CSimpleRibbonApp*)GetApp()); }


#endif
