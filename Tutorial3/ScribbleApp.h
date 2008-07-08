////////////////////////////////////
// ScribbleApp.h


#include "../DevWin++/WinCore.h"
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
