///////////////////////////////////////
// CustomControlApp.h

#ifndef CUSTOMCONTROLAPP_H
#define CUSTOMCONTROLAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CCustomControlApp : public CWinApp
{
public:
	CCustomControlApp();
	virtual ~CCustomControlApp();
	CMyDialog& GetDialog() {return m_MyDialog;}
	virtual BOOL InitInstance();

private:
	CMyDialog m_MyDialog;
};


// returns a reference to the CCustomControlApp object
inline CCustomControlApp& GetDlgApp() { return static_cast<CCustomControlApp&>(GetApp()); }


#endif // define CUSTOMCONTROLAPP_H

