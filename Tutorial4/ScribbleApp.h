////////////////////////////////////
// ScribbleApp.h


#include "../Win32++/WinCore.h"
#include "View.h"


class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
	virtual BOOL InitInstance();

private:
    CView m_View;
};
