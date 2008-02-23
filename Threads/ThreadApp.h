//////////////////////////////////////////////////
// ThreadApp.h
//  Declaration of the CThreadApp class


#ifndef THREADAPP_H
#define THREADAPP_H

#include <string>
#include <sstream>
#include "MainWnd.h"


class CThreadApp : public CWinApp
{
public:
	CThreadApp();
	virtual ~CThreadApp();
	virtual BOOL InitInstance();
	CMainWindow& GetMainWnd() {return m_MainWnd;}

private:
	CMainWindow m_MainWnd;	
};


#endif  //THREADAPP_H

