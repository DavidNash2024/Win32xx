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
    CPictureApp() = default;
    virtual ~CPictureApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;
    virtual BOOL OnIdle(LONG) override;

private:
    CPictureApp(const CPictureApp&) = delete;
    CPictureApp& operator=(const CPictureApp&) = delete;

    CMainFrame m_frame;
};


#endif // define PICTUREAPP_H
