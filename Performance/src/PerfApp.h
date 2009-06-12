//////////////////////////////////////////////////
// PerfApp.h
//  Declaration of the CPerformanceApp class



#ifndef PERFAPP_H
#define PERFAPP_H

#include <sstream>
#include "MainWnd.h"


typedef std::basic_stringstream<TCHAR> tStringStream;


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


// returns a reference to the CPerformanceApp object
inline CPerformanceApp& GetPerfApp() { return *((CPerformanceApp*)GetApp()); }


#endif  //PERFAPP_H

