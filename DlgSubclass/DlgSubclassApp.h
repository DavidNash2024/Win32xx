///////////////////////////////////////
// DlgSubclassApp.h

#ifndef DLGSUBCLASSAPP_H
#define DLGSUBCLASSAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CDlgSubclassApp : public CWinApp
{
public:
	CDlgSubclassApp(HINSTANCE hInstance);
	virtual ~CDlgSubclassApp();
	CMyDialog& GetDialog() {return m_MyDialog;}

private:
	CMyDialog m_MyDialog;
};


#endif // define DLGSUBCLASSAPP_H

