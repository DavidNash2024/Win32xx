/////////////////////////////
// PictureApp.h
//

#ifndef PICTUREAPP_H
#define PICTUREAPP_H

#include "Mainfrm.h"


///////////////////////////////////////
// Declaration of the CPictureApp class
//
class CPictureApp : public CWinApp
{
public:
    CPictureApp();
    virtual ~CPictureApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define PICTUREAPP_H

