/////////////////////////////
// MovieShowApp.h
//

#ifndef _MOVIESHOWAPP_H_
#define _MOVIESHOWAPP_H_

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////////
// CMovieShowApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CMovieShowApp : public CWinApp
{
public:
    CMovieShowApp();
    virtual ~CMovieShowApp() override;
    CMovieShowApp(const CMovieShowApp&) = delete;               // Disable copy construction
    CMovieShowApp& operator=(const CMovieShowApp&) = delete;    // Disable assignment operator

protected:
    virtual BOOL InitInstance() override;

private:
    CMainFrame m_frame;
};


#endif // _MOVIESHOWAPP_H_
