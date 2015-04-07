/////////////////////////////////////////
// RoundApp.h

#ifndef ROUNDAPP_H
#define ROUNDAPP_H

#include "View.h"


// Declaration of the CRoundApp class
class CRoundApp : public CWinApp
{
public:
    CRoundApp();
    virtual ~CRoundApp() {}
	virtual BOOL InitInstance();
	CView* GetView() { return &m_View; }

private:
    CView m_View;
};


// returns a pointer to the CRoundApp object
inline CRoundApp* GetSimpleApp() { return static_cast<CRoundApp*>(GetApp()); }


#endif
