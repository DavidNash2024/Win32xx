//////////////////////////////////////////////////
// PerfApp.h
//  Declaration of the CPerformanceApp class


// Adjust this to change the number of threads created
//#define MAX_THREADS 100

#ifndef THREADAPP_H
#define THREADAPP_H


#include "MainWnd.h"


class CPerformanceApp : public CWinApp
{
public:
	CPerformanceApp(HINSTANCE hInstance);
	virtual ~CPerformanceApp();
	CMainWindow& GetMainWnd() {return m_MainWnd;}

private:
	CMainWindow m_MainWnd;	
};


#endif  //PERFAPP_H

