//////////////////////////////////////////////////
// App.h
//  Declaration of the CApp class


#ifndef TEXTAPP_H
#define TEXTAPP_H

#include "Mainfrm.h"


class CTextApp : public CWinApp
{
public:
	CTextApp(HINSTANCE hInstance); 
	virtual ~CTextApp()  {}

private:
	CMainFrame m_Frame;
};


#endif // define TEXTAPP_H

