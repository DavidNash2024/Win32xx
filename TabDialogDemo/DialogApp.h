///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
	CDialogApp(HINSTANCE hInstance); 
	virtual ~CDialogApp();
	virtual BOOL InitInstance();
	CMyDialog& GetDialog() {return m_MyDialog;}

private:
	CMyDialog m_MyDialog;
};


#endif // define DIALOGAPP_H

