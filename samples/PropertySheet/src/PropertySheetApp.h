/////////////////////////////
// PropertySheetApp.h
//

#ifndef PROPERTYSHEETAPP_H
#define PROPERTYSHEETAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////////////////////////
// CPropertySheetApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// frame window when it runs.
class CPropertySheetApp : public CWinApp
{
public:
    CPropertySheetApp();
    virtual ~CPropertySheetApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define PROPERTYSHEETAPP_H
