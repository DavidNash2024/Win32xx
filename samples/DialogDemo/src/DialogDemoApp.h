///////////////////////////////////////
// DlgSubclassApp.h

#ifndef DLGSUBCLASSAPP_H
#define DLGSUBCLASSAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CDialogDemoApp : public CWinApp
{
public:
	CDialogDemoApp();
	virtual ~CDialogDemoApp();
	CMyDialog& GetDialog() {return m_MyDialog;}
	virtual BOOL InitInstance();

private:
	CMyDialog m_MyDialog;
};


// returns a reference to the CDialogDemoApp object
inline CDialogDemoApp& GetDlgApp() { return static_cast<CDialogDemoApp&>(GetApp()); }


#endif // define DLGSUBCLASSAPP_H

