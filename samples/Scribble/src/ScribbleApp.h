////////////////////////////////////////
// ScribbleApp.h

#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H


#include "Mainfrm.h"



class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
	virtual BOOL InitInstance();
	CMainFrame* GetMainFrame() { return &m_Frame; }

private:
    CMainFrame m_Frame;
};


// returns a reference to the CScribbleApp object
inline CScribbleApp* GetScribbleApp() { return static_cast<CScribbleApp*>(GetApp()); }


#endif //SCRIBBLEAPP_H

