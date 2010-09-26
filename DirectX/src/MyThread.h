//////////////////////////////////////////////////////
// MyThread.h
//  Declaration of the CMyThread class

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "View.h"


class CMyThread : public CWinThread
{
public:
	CMyThread();
	virtual ~CMyThread() {};
	virtual void StartThread(HWND hwndParent);
	virtual BOOL InitInstance();
	virtual int MessageLoop();
	virtual CWnd& GetView() {return m_View;}

private:
	CView m_View;	// The CWnd object for this thread
	HWND m_hwndParent;
};

#endif // MYTHREAD_H

