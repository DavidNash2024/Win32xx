/////////////////////////////
// MovieShowApp.h
//

#ifndef _MOVIESHOWAPP_H_
#define _MOVIESHOWAPP_H_

#include "Mainfrm.h"

////////////////////////////////////////////////
// Declaration of the CMovieShowApp class
// Starts the application and creates the frame.
//
class CMovieShowApp : public CWinApp
{
public:
    CMovieShowApp();
    virtual ~CMovieShowApp();

protected:
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // _MOVIESHOWAPP_H_
