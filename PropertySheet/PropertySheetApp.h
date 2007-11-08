//////////////////////////////////////////////////
// PropertySheetApp.h

#ifndef PROPERTYSHEETAPP_H
#define PROPERTYSHEETAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CPropertySheetApp : public CWinApp
{
public:
	CPropertySheetApp(HINSTANCE hInstance);
	virtual ~CPropertySheetApp();
	virtual BOOL InitInstance();

private:
	CMainFrame m_Frame;
};


#endif // define PROPERTYSHEETAPP_H

