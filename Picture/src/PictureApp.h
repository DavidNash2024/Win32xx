//////////////////////////////////////////////////
// PictureApp.h

#ifndef PICTUREAPP_H
#define PICTUREAPP_H

#include "Mainfrm.h"


// Declaration of the CPictureApp class
class CPictureApp : public CWinApp
{
public:
	CPictureApp();
	virtual ~CPictureApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CPictureApp object
inline CPictureApp& GetPicApp() { return *((CPictureApp*)GetApp()); }


#endif // define PICTUREAPP_H

