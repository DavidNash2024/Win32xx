///////////////////////////////////////
// DlgSubclassApp.h

#ifndef DLGSUBCLASSAPP_H
#define DLGSUBCLASSAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CDlgSubclassApp : public CWinApp
{
public:
	CDlgSubclassApp();
	virtual ~CDlgSubclassApp();
	CMyDialog& GetDialog() {return m_MyDialog;}
	virtual BOOL InitInstance();

private:
	CMyDialog m_MyDialog;
};


// returns a reference to the CDlgSubclassApp object
inline CDlgSubclassApp& GetDlgApp() { return *((CDlgSubclassApp*)GetApp()); }


#endif // define DLGSUBCLASSAPP_H

