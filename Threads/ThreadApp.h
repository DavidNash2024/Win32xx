//////////////////////////////////////////////////
// ThreadApp.h
//  Declaration of the CThreadApp class


// Adjust this to change the number of threads created
#define MAX_THREADS 100

#ifndef THREADAPP_H
#define THREADAPP_H


#include "MainWnd.h"


class CThreadApp : public CWinApp
{
public:
	CThreadApp(HINSTANCE hInstance);
	virtual ~CThreadApp();
	CMainWindow& GetMainWnd() {return m_MainWnd;}

private:
	CMainWindow m_MainWnd;	
};


#endif  //THREADAPP_H

