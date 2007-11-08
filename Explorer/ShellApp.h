//////////////////////////////////////////////////
// ShellApp.h
//  Declaration of the CShellApp class


#ifndef SHELLAPP_H
#define SHELLAPP_H


#include "Mainfrm.h"


class CShellApp : public CWinApp
{
public:
	CShellApp(HINSTANCE hInstance);
	virtual ~CShellApp();
	virtual BOOL InitInstance();

private:
	CMainFrame m_Frame;
};


#endif  //SHELLAPP_H

