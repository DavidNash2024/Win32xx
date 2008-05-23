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
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CSplitterApp object
inline CSplitterApp& GetSplitApp() { return *((CSplitterApp*)GetApp()); }


#endif // define EFRAMEAPP_H

