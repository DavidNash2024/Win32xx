///////////////////////////////////////
// CustomControlApp.h

#ifndef CUSTOMCONTROLAPP_H
#define CUSTOMCONTROLAPP_H

#include "MyDialog.h"
#include "MyWebControl.h"


// Declaration of the CDialogApp class
class CCustomControlApp : public CWinApp
{
public:
	CCustomControlApp();
	virtual ~CCustomControlApp();
	CMyDialog* GetDialog() {return &m_MyDialog;}
	virtual BOOL InitInstance();

private:
	CMyDialog m_MyDialog;
};


// returns a pointer to the CCustomControlApp object
inline CCustomControlApp* GetDlgApp() { return (CCustomControlApp*)GetApp(); }


#endif // define CUSTOMCONTROLAPP_H

