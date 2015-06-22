///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
	CDialogApp(); 
	virtual ~CDialogApp();
	virtual BOOL InitInstance();
	CMyDialog& GetDialog() {return m_MyDialog;}

private:
	CMyDialog m_MyDialog;
};

// returns a reference to the CDialogApp object
inline CDialogApp& GetDialogApp() { return static_cast<CDialogApp&>(GetApp()); }


#endif // define DIALOGAPP_H

