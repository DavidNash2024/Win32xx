//////////////////////////////////////////////////
// SubApp.h
//  Declaration of the CSubApp class

#ifndef SUBAPP_H
#define SUBAPP_H

#include "MainWin.h"


class CSubApp : public CWinApp
{
public:
	CSubApp(HINSTANCE hInstance);
	virtual ~CSubApp();
	virtual InitInstance();

private:
	CMainWin m_Win;
};


#endif // define SUBAPP_H

