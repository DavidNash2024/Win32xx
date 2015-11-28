//////////////////////////////////////////////////
// SplitterApp.h
//  Declaration of the CApp class

#ifndef SPLITTERAPP_H
#define SPLITTERAPP_H

#include "Mainfrm.h"


class CSplitterApp : public CWinApp
{
public:
	CSplitterApp();
	virtual ~CSplitterApp()  {}
	virtual BOOL InitInstance();
	CMainDockFrame& GetMainFrame() { return m_Frame; }

private:
	CMainDockFrame m_Frame;
};


// returns a pointer to the CSplitterApp object
inline CSplitterApp& GetSplitApp() { return static_cast<CSplitterApp&>(GetApp()); }


#endif // define EFRAMEAPP_H

