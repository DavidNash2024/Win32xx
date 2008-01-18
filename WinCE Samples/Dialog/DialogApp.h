//////////////////////////////////////////////////
// DialogApp.h
//  Declaration of the CDialogApp class

#ifndef DIALOGAPP_H
#define DIALOGAPP_H


#include "..\Win32++\dialog.h"
#include "MyDialog.h"


class CDialogApp : public CWinApp
{
public:
	CDialogApp(HINSTANCE hInstance);
	virtual ~CDialogApp();
	virtual BOOL InitInstance();

private:
	CMyDialog MyDialog;
};


#endif // define DIALOGAPP_H

