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

private:
	CMyDialog MyDialog;
};


#endif // define DIALOGAPP_H

