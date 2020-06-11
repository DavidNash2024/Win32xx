//////////////////////////////////////////////////
// MovieShowApp.h

#ifndef MOVIESHOWAPP_H
#define MOVIESHOWAPP_H

#include "Mainfrm.h"


// Declaration of the CMovieShowApp class
// Starts the application and creates the frame.
class CMovieShowApp : public CWinApp
{
public:
    CMovieShowApp();
    virtual ~CMovieShowApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


//returns a pointer to the CFrameApp object
inline CMovieShowApp* GetMovieShowApp() { return static_cast<CMovieShowApp*>(GetApp()); }


#endif // define MOVIESHOWAPP_H

