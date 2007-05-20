//////////////////////////////////////////////////
// PerfApp.h
//  Declaration of the CPerformanceApp class



#ifndef PERFAPP_H
#define PERFAPP_H

#include <string>
#include <sstream>
#include "MainWnd.h"

typedef std::basic_string<TCHAR> tString;
typedef std::basic_stringstream<TCHAR> tStringStream;


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

