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

private:
    CMainFrame m_frame;
};


#endif // define PROPERTYSHEETAPP_H
