/////////////////////////////
// FrameApp.h
//

#pragma once

#include "mainfrm.h"

/////////////////////////////////////
// Declaration of the CFrameApp class
//
class CFrameApp : public CWinApp
{
public:
    CFrameApp();
    virtual ~CFrameApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};

