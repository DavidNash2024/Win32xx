//////////////////////////////////////////////////
// PictureApp.h

#ifndef PICTUREAPP_H
#define PICTUREAPP_H

#include "Mainfrm.h"


// Declaration of the CPictureApp class
class CPictureApp : public CWinApp
{
public:
	CPictureApp(HINSTANCE hInstance);
	virtual ~CPictureApp();

private:
	CMainFrame m_Frame;
};


#endif // define PICTUREAPP_H

