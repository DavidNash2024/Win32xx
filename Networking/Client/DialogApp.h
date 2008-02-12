///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "ClientDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
	CDialogApp(HINSTANCE hInstance); 
	virtual ~CDialogApp();
	virtual BOOL InitInstance();
	CClientDialog& GetDialog() {return m_ClientDialog;}

private:
	CClientDialog m_ClientDialog;
};


#endif // define DIALOGAPP_H

