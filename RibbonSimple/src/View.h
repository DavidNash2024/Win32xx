/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include <wincore.h>
#include <ribbon.h>


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}
	void DestroyRibbon();
	bool CreateRibbon();
	virtual HRESULT RibbonExecute(UINT nCmdID, UINT verb, LPCVOID key, LPCVOID ppropvarValue, LPVOID pCommandExecutionProperties);

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CRibbonApplication* m_pRibbonApp;
	IUIFramework* m_pRibbonFramework;
};

#endif
