//////////////////////////////////////////////
// ViewThread.h

#ifndef VIEWTHREAD_H
#define VIEWTHREAD_H

#include "View.h"

class CViewThread : public CWinThread
{
public:
	CViewThread() {}
	virtual ~CViewThread() {}

	virtual BOOL InitInstance();
	virtual int MessageLoop();
	CView& GetView() { return m_View; }

private:
	CView m_View;		// CMainFrame's View window
};


#endif
