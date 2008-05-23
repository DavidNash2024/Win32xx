///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "SvrDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
	CDialogApp(); 
	virtual ~CDialogApp();
	virtual BOOL InitInstance();
	CSvrDialog& GetDialog() {return m_SvrDialog;}

private:
	CSvrDialog m_SvrDialog;
};


// returns a reference to the CDialogApp object
inline CDialogApp& GetDlgApp() { return *((CDialogApp*)GetApp()); }


#endif // define DIALOGAPP_H

