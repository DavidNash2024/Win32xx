//////////////////////////////////////////////////
// SuperclassApp.h
//  Declaration of the CSubApp class

#ifndef SUPERCLASSAPP_H
#define SUPERCLASSAPP_H

#include "MainWin.h"


class CSuperclassApp : public CWinApp
{
public:
	CSuperclassApp(HINSTANCE hInstance);
	virtual ~CSuperclassApp();

private:
	CMainWin m_Win;
};


#endif // define SUPERCLASSAPP_H

