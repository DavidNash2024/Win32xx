//////////////////////////////////////////////////
// SubclassApp.h
//  Declaration of the CSubApp class

#ifndef SUBCLASSAPP_H
#define SUBCLASSAPP_H

#include "MainWin.h"


////////////////////////////////////////////////////////////
// CSimpleApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main dialog when it runs.
class CSubclassApp : public CWinApp
{
public:
    CSubclassApp();
    virtual ~CSubclassApp();
    virtual BOOL InitInstance();
    CMainWin& GetWin() { return m_win; }

private:
    CMainWin m_win;
};


// returns a pointer to the CSubclassApp object
inline CSubclassApp* GetSubApp() { return static_cast<CSubclassApp*>(GetApp()); }


#endif // define SUBCLASSAPP_H

