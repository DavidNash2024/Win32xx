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
    virtual ~CMovieShowApp();

protected:
    virtual BOOL InitInstance();

private:
    CMovieShowApp(const CMovieShowApp&);                // Disable copy construction
    CMovieShowApp& operator = (const CMovieShowApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // _MOVIESHOWAPP_H_
