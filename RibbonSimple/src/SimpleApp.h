/////////////////////////////////////////
// SimpleApp.h

#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "View.h"


// Declaration of the CWinApp class
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp();
	virtual BOOL InitInstance();
	CView& GetView() { return m_View; }

private:
    CView m_View;
};


// returns a reference to the CSimpleApp object
inline CSimpleApp& GetSimpleApp() { return *((CSimpleApp*)GetApp()); }


#endif
