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
	void SetFrame(CFrame* pFrame) { m_pFrame = pFrame; }

private:
	CView m_View;		// CMainFrame's View window
	CFrame* m_pFrame;	// Pointer to CMainFrame
};


#endif
