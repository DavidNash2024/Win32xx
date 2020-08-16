/////////////////////////////
// PropertySheetApp.h
//

#ifndef PROPERTYSHEETAPP_H
#define PROPERTYSHEETAPP_H

#include "Mainfrm.h"

/////////////////////////////////////////////
// Declaration of the CPropertySheetApp class
//
class CPropertySheetApp : public CWinApp
{
public:
    CPropertySheetApp();
    virtual ~CPropertySheetApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CPropertySheetApp object
inline CPropertySheetApp* GetPSApp() { return static_cast<CPropertySheetApp*>(GetApp()); }


#endif // define PROPERTYSHEETAPP_H

