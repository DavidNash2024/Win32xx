/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include "wincore.h"
#include "Application.h"

class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CApplication* m_pApplication;
	IUIFramework* m_pFramework;
};

#endif
