//////////////////////////////////////////////////
// ShellApp.h
//  Declaration of the CShellApp class


#ifndef SHELLAPP_H
#define SHELLAPP_H


#include "Mainfrm.h"


class CShellApp : public CWinApp
{
public:
	CShellApp();
	virtual ~CShellApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};

// returns a reference to the CShellApp object
inline CShellApp& GetShellApp() { return *((CShellApp*)GetApp()); }


#endif  //SHELLAPP_H

