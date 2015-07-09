///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "ClientDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
	CDialogApp(); 
	virtual ~CDialogApp();
	virtual BOOL InitInstance();
	CClientDialog& GetDialog() {return m_ClientDialog;}

private:
	CClientDialog m_ClientDialog;
};


// returns a reference to the CDialogApp object
inline CDialogApp& GetDlgApp() { return static_cast<CDialogApp&>(GetApp()); }


#endif // define DIALOGAPP_H

