//////////////////////////////////////////////////
// PropertySheetApp.h

#ifndef PROPERTYSHEETAPP_H
#define PROPERTYSHEETAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CPropertySheetApp : public CWinApp
{
public:
	CPropertySheetApp();
	virtual ~CPropertySheetApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CPropertySheetApp object
inline CPropertySheetApp& GetPSApp() { return *((CPropertySheetApp*)GetApp()); }


#endif // define PROPERTYSHEETAPP_H

