//////////////////////////////////////////////////
// PerfApp.h
//  Declaration of the CPerformanceApp class



#ifndef PERFAPP_H
#define PERFAPP_H

#include <string>
#include <sstream>
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

