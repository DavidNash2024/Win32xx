//////////////////////////////////////////////
//MovieShowApp.cpp

#include "stdafx.h"
#include "MovieShowApp.h"


// Definitions for the CMovieShowApp class
CMovieShowApp::CMovieShowApp()
{
}

CMovieShowApp::~CMovieShowApp()
{
}

BOOL CMovieShowApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure    

    return TRUE;
}


