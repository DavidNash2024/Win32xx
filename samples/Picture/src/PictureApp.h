/////////////////////////////
// PictureApp.h
//

#ifndef PICTUREAPP_H
#define PICTUREAPP_H

#include "Mainfrm.h"


//////////////////////////////////////////////////////////////////
// CPictureApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
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

