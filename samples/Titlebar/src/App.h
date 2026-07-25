/////////////////////////////
// App.h
//

#ifndef APP_H_
#define APP_H_

#include "wxx_wincore.h"
#include "MiniFrame.h"


////////////////////////////////////////////////////////////
// CApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main
// window when it runs.
class CApp : public CWinApp
{
public:
    CApp() = default;
    virtual ~CApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

    CMiniFrame m_view;
};


#endif  // APP_H_
