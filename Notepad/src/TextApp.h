//////////////////////////////////////////////////
// TextApp.h
//  Declaration of the CTextApp class

#ifndef TEXTAPP_H
#define TEXTAPP_H

#include "Mainfrm.h"


class CTextApp : public CWinApp
{
public:
	CTextApp(); 
	virtual ~CTextApp()  {}
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CTextApp object
inline CTextApp& GetTextApp() { return *((CTextApp*)GetApp()); }


#endif // define TEXTAPP_H

