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

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CPictureApp(const CPictureApp&);                // Disable copy construction
    CPictureApp& operator = (const CPictureApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // define PICTUREAPP_H
