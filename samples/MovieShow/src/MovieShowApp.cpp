/////////////////////////////
//MovieShowApp.cpp
//

#include "stdafx.h"
#include "MovieShowApp.h"

//////////////////////////////////////
// CMovieShowApp function definitions
//

// Constructor
CMovieShowApp::CMovieShowApp()
{
}

// Destructor
CMovieShowApp::~CMovieShowApp()
{
}

// Called when the application starts.
BOOL CMovieShowApp::InitInstance()
{
    // Create the frame window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


