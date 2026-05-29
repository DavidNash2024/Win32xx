/////////////////////////////////////////
// App.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef THE_APP_H
#define THE_APP_H

#include "Mainfrm.h"

/////////////////////////////////////////////////////////////////////////
// This class manages the application. It initializes the application and
// creates the main frame window when the application starts.
class CApp : public CWinApp
{
public:
    CApp() = default;
    virtual ~CApp() override = default;

    virtual BOOL    InitInstance() override;
    CMainFrame& TheFrame() { return m_frame; }

private:
    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

    CMainFrame m_frame;
};


// Return a reference to the CApp object.
inline CApp* TheApp()
{
    return static_cast<CApp*>(GetApp());
}

// Return a reference to the CMainFrame object.
inline CMainFrame& GetFrame()
{
    return TheApp()->TheFrame();
}

// Return a reference to the rich view object being previewed.
inline CRichView& GetRichView()
{
    return GetFrame().GetRichView();
}

#endif // THE_APP_H
