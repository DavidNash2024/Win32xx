//////////////////////////////////////////////////
// SubApp.h
//  Declaration of the CSubApp class

#ifndef SUBAPP_H
#define SUBAPP_H

#include "MainWin.h"


class CSubApp : public CWinApp
{
public:
	CSubApp();
	virtual ~CSubApp();
	virtual BOOL InitInstance();
	CMainWin& GetMainWin() { return m_Win; }

private:
	CMainWin m_Win;
};


// returns a reference to the CSubApp object
inline CSubApp& GetSubApp() { return *((CSubApp*)GetApp()); }


#endif // define SUBAPP_H

