//////////////////////////////////////////////////
// ThreadApp.h
//  Declaration of the CThreadApp class


#define MAX_THREADS 100

#ifndef THREADAPP_H
#define THREADAPP_H


#include "MainWin.h"



class CThreadApp : public CWinApp
{
public:
	CThreadApp(HINSTANCE hInstance);
	virtual ~CThreadApp();

private:
	CWnd m_MainWin;
};


#endif  //THREADAPP_H

