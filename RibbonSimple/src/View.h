/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include <wincore.h>
#include <ribbon.h>


class CView : public CWnd
{
public:
	CView() : m_uRibbonHeight(0) {}
	virtual ~CView() {}
	virtual HRESULT RibbonExecute(UINT nCmdID, UINT verb, LPCVOID key, LPCVOID ppropvarValue, LPVOID pCommandExecutionProperties);
	virtual HRESULT RibbonOnViewChanged(UINT viewId, UINT typeId, void* pView, UINT verb, INT uReasonCode);

	UINT GetRibbonHeight() { return m_uRibbonHeight; }

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CRibbon m_Ribbon;
	UINT m_uRibbonHeight;

};

#endif
