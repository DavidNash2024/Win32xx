//////////////////////////////////////////////////
// ThreadApp.h
//  Declaration of the CThreadApp class


// Adjust this to change the number of threads created
//#define MAX_THREADS 100

#ifndef THREADAPP_H
#define THREADAPP_H

#include <string>
#include <sstream>
#include "MainWnd.h"

typedef std::basic_string<TCHAR> tString;
typedef std::basic_stringstream<TCHAR> tStringStream;


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

