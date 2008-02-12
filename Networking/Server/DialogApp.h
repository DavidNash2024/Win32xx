///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "SvrDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
	CDialogApp(HINSTANCE hInstance); 
	virtual ~CDialogApp();
	virtual BOOL InitInstance();
	CSvrDialog& GetDialog() {return m_SvrDialog;}

private:
	CSvrDialog m_SvrDialog;
};


#endif // define DIALOGAPP_H

