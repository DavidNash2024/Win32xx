//////////////////////////////////////////////////
// SubclassApp.h
//  Declaration of the CSubApp class

#ifndef SUBCLASSAPP_H
#define SUBCLASSAPP_H

#include "MainWin.h"


class CSubclassApp : public CWinApp
{
public:
	CSubclassApp();
	virtual ~CSubclassApp();
	virtual BOOL InitInstance();

private:
	CMainWin m_Win;
};


#endif // define SUBCLASSAPP_H

