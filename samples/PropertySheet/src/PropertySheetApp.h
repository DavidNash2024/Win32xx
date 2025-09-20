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
    CPropertySheetApp() = default;
    virtual ~CPropertySheetApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CPropertySheetApp(const CPropertySheetApp&) = delete;
    CPropertySheetApp& operator=(const CPropertySheetApp&) = delete;

    CMainFrame m_frame;
};


#endif // define PROPERTYSHEETAPP_H
