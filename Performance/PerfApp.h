//////////////////////////////////////////////////
// PerfApp.h
//  Declaration of the CPerformanceApp class



#ifndef PERFAPP_H
#define PERFAPP_H

#include "MainWnd.h"
#include <string>
#include <sstream>


class CPerformanceApp : public CWinApp
{
public:
	CPerformanceApp();
	virtual ~CPerformanceApp();
	virtual BOOL InitInstance();
	CMainWindow& GetMainWnd() {return m_MainWnd;}

private:
	CMainWindow m_MainWnd;	
};


#endif  //PERFAPP_H

